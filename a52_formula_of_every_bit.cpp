#include <iostream>
#include <bitset>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <limits>

//В A51 алгоритме статье по криптоанализу все ki + 1 нужно выводить то есть не k10 а k11 а регистр крутится в обратном направлении
//если хочу посмотреть формулу в 0 ячейке то это будет у меня в 18 ячейке но + 1

struct HowBitWasFormed {
    bool bit{};
    std::vector<int> formula{};
    HowBitWasFormed() = default;
};

void push_from_one_to_second(std::vector<int>& first, const std::vector<int>& second) {
    for(std::size_t i = 0; i < second.size(); ++i) {
        first.push_back(second[i]);
    }
}

auto generate_key() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long long> dist(0,std::numeric_limits<long long>::max());
    return dist(rd);
}

template<int N>
auto from_integral_type_to_binary(const long long& key) {
    std::vector<long long> ans{};
    auto binary_string = std::bitset<N>(key).to_string();
    for(const auto& x : binary_string) {
        if (x == '1') {
            ans.push_back(1);
        } else {
            ans.push_back(0);
        }
    }
    return ans;
}

void roll(std::vector<HowBitWasFormed>& reg, const std::vector<int>& fbp) {
    auto _bit = 0;
    std::vector<int> _formula;
    for(std::size_t i = 0; i < fbp.size(); ++i) {
        _bit ^= reg[fbp[i]].bit;
        push_from_one_to_second(_formula,reg[fbp[i]].formula); 
    }
    std::rotate(reg.rbegin(),reg.rbegin()+1,reg.rend());
    reg[0].bit = _bit;
    reg[0].formula = _formula;
}


void do_all_for_register(std::vector<HowBitWasFormed>& reg, const std::vector<int>& fbp, std::vector<long long>& key) {
    std::cout << "Step: ";
    std::size_t n{};
    std::cin >> n;
    for(std::size_t i = 0; i < n-1; ++i) {
        if (i < key.size()) {
            reg[0].bit ^= key[i];
            reg[0].formula.push_back(i);
        }
        roll(reg,fbp);
    }
    for(std::size_t i = 0; i < reg.size(); ++i) {
        std::map<int,int> mp;
        std::vector<int> new_formula{};
        auto fixed_vec = reg[i].formula;
        for(std::size_t j = 0; j < fixed_vec.size(); ++j) {
            mp[fixed_vec[j]]++;
        }
        for(const auto& [key,value] : mp) {
               if (value & 1) {
                 new_formula.push_back(key);   
               }
        }
        reg[i].formula = new_formula;
    }
}


void print_formula(std::vector<HowBitWasFormed>& reg, int bit) {
    for(const auto& x : reg[bit].formula) {
        std::cout << "K[" << x << "]" << " + ";
    }
    std::cout << "\n";
}


int main() {

    constexpr int A52_R1_SIZE = 19;
    constexpr int A52_R2_SIZE = 22;
    constexpr int A52_R3_SIZE = 23;

    auto key = from_integral_type_to_binary<64>(generate_key());
        
    std::vector<HowBitWasFormed> A52_R1(A52_R1_SIZE,HowBitWasFormed());
    std::vector<HowBitWasFormed> A52_R2(A52_R2_SIZE,HowBitWasFormed());
    std::vector<HowBitWasFormed> A52_R3(A52_R3_SIZE,HowBitWasFormed());
    
    std::vector<int> A52_R1_fbp{13,16,17,18};
    std::vector<int> A52_R2_fbp{20,21};
    std::vector<int> A52_R3_fbp{7,20,21,22};

    do_all_for_register(A52_R1,A52_R1_fbp,key);
    print_formula(A52_R1,18);


    return EXIT_SUCCESS;
}

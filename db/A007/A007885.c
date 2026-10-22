/*
 * A007885  Numbers n such that balanced sequences exist with n distinct
 *          elements.
 * https://oeis.org/A007885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n-th number that is 1, 2, 4, or a prime p where 2 or -2
 *            is a primitive root modulo p
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


static bool is_primitive_root(uint64_t g, uint64_t p) {
    uint64_t order = p - 1;
    uint64_t d = 2;
    uint64_t temp = order;
    
    while (d * d <= temp) {
        if (temp % d == 0) {
            uint64_t exp = order / d;
            uint64_t result = 1;
            uint64_t base = g % p;
            uint64_t e = exp;
            
            while (e > 0) {
                if (e & 1) {
                    result = ((unsigned __int128)result * base) % p;
                }
                base = ((unsigned __int128)base * base) % p;
                e >>= 1;
            }
            
            if (result == 1) {
                return false;
            }
            
            while (temp % d == 0) {
                temp /= d;
            }
        }
        d++;
    }
    
    if (temp > 1) {
        uint64_t exp = order / temp;
        uint64_t result = 1;
        uint64_t base = g % p;
        uint64_t e = exp;
        
        while (e > 0) {
            if (e & 1) {
                result = ((unsigned __int128)result * base) % p;
            }
            base = ((unsigned __int128)base * base) % p;
            e >>= 1;
        }
        
        if (result == 1) {
            return false;
        }
    }
    
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    
    idx_t count = 2;
    
    for (uint64_t candidate = 3; ; candidate++) {
        bool in_sequence = false;
        
        if (candidate == 4) {
            in_sequence = true;
        } else if (is_prime(candidate)) {
            if (is_primitive_root(2, candidate) || 
                is_primitive_root(candidate - 2, candidate)) {
                in_sequence = true;
            }
        }
        
        if (in_sequence) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
    }
}

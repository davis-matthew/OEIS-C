/*
 * A003975  Inverse Möbius transform of A003962.
 * https://oeis.org/A003975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = (q^(e+1)-1)/(q-1) where q =
 *            (nextPrime(p)+1)/2
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_check, next_prime_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_check.h"
#include "../utils/next_prime_func.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    uint64_t temp = n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            
            uint64_t next_p = next_prime_func(p);
            seq_t q = (seq_t)(((uint64_t)next_p + 1) / 2);
            
            seq_t factor = 1;
            seq_t power = 1;
            for (uint64_t i = 0; i < e; i++) {
                power *= q;
                factor += power;
            }
            
            result *= factor;
        }
    }
    
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t next_p = next_prime_func(p);
        seq_t q = (seq_t)(((uint64_t)next_p + 1) / 2);
        seq_t factor = q + 1;
        result *= factor;
    }
    
    return result;
}

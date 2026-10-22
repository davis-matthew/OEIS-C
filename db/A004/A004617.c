/*
 * A004617  Divisible only by primes congruent to 3 mod 5.
 * https://oeis.org/A004617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers divisible only by primes congruent to 3 mod 5
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   all_prime_factors_3_mod_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/all_prime_factors_3_mod_5.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        if (all_prime_factors_3_mod_5(k)) {
            count++;
        }
    }
    
    return k;
}

/*
 * A004628  Divisible only by primes congruent to 7 mod 8.
 * https://oeis.org/A004628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   all_factors_congruent_7_mod_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integers divisible only by primes congruent to 7 mod 8.
   Search for the n-th such integer. */

#include <stdbool.h>
#include "../utils/all_factors_congruent_7_mod_8.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        if (all_factors_congruent_7_mod_8(m)) {
            count++;
        }
    }
    
    return m;
}

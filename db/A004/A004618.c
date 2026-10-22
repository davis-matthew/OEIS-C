/*
 * A004618  Divisible only by primes congruent to 4 mod 5.
 * https://oeis.org/A004618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sequence of positive integers divisible only by primes congruent
 *            to 4 mod 5. Search for the n-th such number
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_only_primes_4mod5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_only_primes_4mod5.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (has_only_primes_4mod5(candidate)) {
            count++;
        }
    }
    
    return candidate;
}

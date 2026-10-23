/*
 * A045374  Primes congruent to {0, 2, 4} mod 7.
 * https://oeis.org/A045374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = The n-th prime congruent to 0, 2 or 4 modulo 7 (search)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* The sequence is defined only for n ≥ 1. If n == 0 we return 0 as a placeholder. */
    if (n == 0) return (seq_t)0;
    idx_t count    = 0;
    idx_t candidate = 2;
    seq_t result = (seq_t)0;
    while (count < n) {
        if (is_prime(candidate) &&
            (candidate % 7 == 0 || candidate % 7 == 2 || candidate % 7 == 4)) {
            count++;
            if (count == n) {
                result = (seq_t)candidate;
                break;
            }
        }
        candidate++;
    }
    return result;
}

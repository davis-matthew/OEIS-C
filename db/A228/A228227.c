/*
 * A228227  Primes congruent to {7, 11} mod 16.
 * https://oeis.org/A228227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes congruent to 7 or 11 (mod 16). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t count = 0;
    idx_t k = 0;
    seq_t cand;

    while (1) {
        /* First candidate: 16·k + 7 */
        cand = (seq_t)16 * (seq_t)k + (seq_t)7;
        if (is_prime_big(cand)) {
            count++;
            if (count == n)
                return cand;
        }

        /* Second candidate: 16·k + 11 */
        cand = (seq_t)16 * (seq_t)k + (seq_t)11;
        if (is_prime_big(cand)) {
            count++;
            if (count == n)
                return cand;
        }

        k++;
    }
}

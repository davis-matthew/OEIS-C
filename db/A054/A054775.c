/*
 * A054775  Positive multiples of 6 which are not the midpoint of a pair of
 *          twin primes.
 * https://oeis.org/A054775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: positive multiples of 6 that are not the midpoint of a pair of twin primes. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;      /* how many valid multiples have been found */
    idx_t k = 1;          /* k = multiple/6 */

    for (;;) {
        seq_t cand = 6 * (seq_t)k;          /* current multiple of 6 */

        /* cand is excluded if cand-1 and cand+1 are both prime (twin‑prime midpoint) */
        if (!(is_prime((idx_t)(cand - 1)) && is_prime((idx_t)(cand + 1)))) {
            ++count;
            if (count == n)
                return cand;
        }

        ++k;
    }
}

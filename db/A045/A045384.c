/*
 * A045384  Primes congruent to {1, 2, 4, 6} mod 7.
 * https://oeis.org/A045384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 1, 2, 4 or 6 modulo 7.
   a(1) = 2.  The function returns the n‑th such prime. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1 – undefined for n < 1 */
        return (seq_t)0;

    /* a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    /* Search for the n-th prime satisfying the congruence conditions. */
    idx_t  count   = 1;              /* prime 2 already counted */
    uint64_t cand  = 3;              /* start with the first odd candidate */

    while (count < n) {
        /* Check congruence modulo 7. */
        uint64_t r = cand % 7ULL;
        if (r == 1ULL || r == 2ULL || r == 4ULL || r == 6ULL) {
            if (is_prime_big((seq_t)cand)) {
                ++count;
                if (count == n)
                    return (seq_t)cand;
            }
        }
        cand += 2;   /* proceed to the next odd number */
    }

    /* Should never be reached for n >= 1. */
    return (seq_t)0;
}

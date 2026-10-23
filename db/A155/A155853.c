/*
 * A155853  Numbers n such that 13*n + 3 is a prime.
 * https://oeis.org/A155853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 473, u32 n <= 525, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 13*n + 3 is a prime. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined offset – return 0 */

    idx_t cand   = 0;      /* candidate n */
    idx_t count  = 0;      /* how many valid n's seen so far */

    for (;; ++cand) {
        /* Check if 13*cand + 3 is prime. */
        seq_t test = (seq_t)13 * (seq_t)cand + (seq_t)3;
        if (is_prime_big(test)) {
            ++count;
            if (count == n)
                return (seq_t)cand;   /* found the nth member */
        }
    }
}

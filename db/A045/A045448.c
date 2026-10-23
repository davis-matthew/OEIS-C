/*
 * A045448  Primes congruent to {1, 4, 5, 6} mod 7.
 * https://oeis.org/A045448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p with p mod 7 ∈ {1,4,5,6}
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)                          /* offset is 1; undefined for n < 1 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t cand   = 2;                    /* start checking from 2 */

    while (1) {
        if (is_prime(cand)) {
            idx_t r = cand % 7;
            if (r == 1 || r == 4 || r == 5 || r == 6) {
                ++count;
                if (count == n)
                    return (seq_t)cand;
            }
        }
        ++cand;                          /* next integer */
    }
}

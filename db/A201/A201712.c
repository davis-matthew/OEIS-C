/*
 * A201712  Primes of the form 2n^2 - 3.
 * https://oeis.org/A201712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 4762, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 54 terms through n = 4630 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: primes of the form 2*n^2-3 */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 2; /* start with smallest k yielding a positive value */
    while (1)
    {
        seq_t cand = ((seq_t)k) * (seq_t)k * (seq_t)2 - (seq_t)3;
        if (is_prime_seq(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        ++k;
    }
}

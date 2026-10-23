/*
 * A133312  a(n) is the first pentagonal number that is nontrivially the sum
 *          of two pentagonal numbers of the type P(p) + P(p+n) (we always
 *          have P(k) = P(0) + P(k)).
 * https://oeis.org/A133312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the pentagonal number P(k) that is the first nontrivial *
 *            sum P(p)+P(p+n-1), i.e. the smallest k with * P(k) = P(p) +
 *            P(p+n-1) for some p>=1. We compute k by * solving (6k-1)^2 =
 *            2*(6p+3(n-1)-1)^2 + 18*(n-1)^2 - 1. For each n * we search p
 *            from 1 upward until an integer k is found. The val...
 * Range:     u8 none, u16 n <= 1, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* shift = n - 1 (the difference between the two summands) */
    seq_t shift = (seq_t)n - 1;

    for (idx_t p = 1; ; ++p)
    {
        seq_t p_s   = (seq_t)p;
        seq_t A     = (seq_t)6 * p_s + (seq_t)3 * shift - 1;
        seq_t RHS   = (seq_t)2 * A * A + (seq_t)18 * shift * shift - 1;
        seq_t B     = floor_sqrt(RHS);

        if (B * B == RHS)
        {
            seq_t B_plus_one = B + 1;
            if (B_plus_one % 6 == 0)
            {
                seq_t k = B_plus_one / 6;           /* index of the resulting pentagonal number */
                return k * ((seq_t)3 * k - 1) / 2;  /* P(k) */
            }
        }
    }
}

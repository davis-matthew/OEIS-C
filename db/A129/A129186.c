/*
 * A129186  Right shift operator generating 1's in shifted spaces.
 * https://oeis.org/A129186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if n = t(t+1)/2 + t - 1 for some integer t ≥ 0,
 *            otherwise 0 Equivalent: a(0)=1; for n>0, let s = 8*n + 17; a(n)
 *            = 1 iff s is a perfect square and (sqrt(s)-3) is even
 * Range:     u8 n <= 29, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   sqrt_floor_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor_seq_t.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* s = 8*n + 17  (sequence property) */
    seq_t s = ((seq_t)8 * n) + 17;

    /* compute integer sqrt of s */
    seq_t r = sqrt_floor_seq_t(s);

    /* check perfect square and parity condition */
    if (r * r == s && ((r - 3) & 1) == 0)
        return 1;
    else
        return 0;
}

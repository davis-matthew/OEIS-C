/*
 * A190373  a(n) = n + floor(n*r/s) + floor(n*t/s) + floor(n*u/s), where r =
 *          sin(Pi/5), s = 1/r, t = sin(2*Pi/5), u = 1/t.
 * https://oeis.org/A190373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊ n*(5-√5)/8 ⌋ + ⌊ n*√5/4 ⌋ + ⌊ n*(√5-1)/2 ⌋
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;   /* safety, the sequence is defined for n ≥ 1 */

    seq_t nn = (seq_t)n;

    /* compute m = ⌊ n*√5 ⌋ as ⌊ √(5*n²) ⌋ using the provided floor_sqrt */
    seq_t m = floor_sqrt(((seq_t)5) * nn * nn);

    /* ⌊ n*(5-√5)/8 ⌋  →  ⌊ (5n - m)/8 ⌋  with a special case when the numerator
       is exactly divisible by 8, we need to subtract 1 because the true value
       is slightly smaller than the integer division because of the missing
       fractional part of n*√5. */
    seq_t a1_num = ((seq_t)5) * nn - m;
    seq_t term1  = a1_num / 8;
    if (a1_num % 8 == 0) term1 -= 1;   /* adjust for exact division */

    /* ⌊ n*√5/4 ⌋  →  ⌊ m/4 ⌋  (no adjustment needed) */
    seq_t term2 = m / 4;

    /* ⌊ n*(√5-1)/2 ⌋  →  ⌊ (m - n)/2 ⌋  (again no adjustment necessary) */
    seq_t term3 = (m - nn) / 2;

    return nn + term1 + term2 + term3;
}

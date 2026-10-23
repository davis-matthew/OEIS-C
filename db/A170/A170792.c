/*
 * A170792  a(n) = n^9*(n^10 + 1)/2.
 * https://oeis.org/A170792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^10 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x    = (seq_t)n;
    seq_t p9   = 1;
    for (idx_t i = 0; i < 9; ++i)
        p9 *= x;                 /* n^9 */

    seq_t p10  = p9 * x;          /* n^10 */

    seq_t term1 = p9;
    seq_t term2 = p10 + 1;

    /* divide the even factor by 2 before multiplying to avoid overflow */
    if ((term1 % 2) == 0)
        term1 /= 2;
    else
        term2 /= 2;

    return term1 * term2;
}

/*
 * A060736  Array of square numbers read by antidiagonals in up direction.
 * https://oeis.org/A060736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i^2-j+1 if i>=j, else (j-1)^2+i, where i = n - t*(t+1)/2,
 *            j = ((t*t)+3*t+4)/2 - n, t = floor((sqrt(8*n-7)-1)/2)
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Safety: n >= 1 is assumed by OEIS offset. */
    seq_t nn = (seq_t)n;

    /* t = floor((sqrt(8*n-7)-1)/2) */
    seq_t t = (floor_sqrt(((seq_t)8 * nn) - (seq_t)7) - (seq_t)1) / (seq_t)2;

    /* i = n - t*(t+1)/2 */
    seq_t tri = t * (t + (seq_t)1) / (seq_t)2;
    seq_t i = nn - tri;

    /* j = ((t*t)+3*t+4)/2 - n */
    seq_t num = t * t + (seq_t)3 * t + (seq_t)4;
    seq_t tri2 = num / (seq_t)2;
    seq_t j = tri2 - nn;

    if (i >= j)
        return i * i - j + (seq_t)1;
    else
        return (j - (seq_t)1) * (j - (seq_t)1) + i;
}

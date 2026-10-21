/*
 * A052153  Rhombi (in 3 different orientations) in a rhombus with 60-degree
 *          acute angles.
 * https://oeis.org/A052153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2·n³ + 15·n² – 11·n)/6 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)      /* offset is 1 – return 0 for n==0 */
        return 0;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n²  */
    seq_t N3 = N2 * N;         /* n³  */

    seq_t term = ( (seq_t)2 * N3 ) + ( (seq_t)15 * N2 ) - ( (seq_t)11 * N );
    return term / (seq_t)6;    /* division is exact */
}

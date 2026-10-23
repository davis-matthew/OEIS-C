/*
 * A033953  Write 0,1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 0 at the
 *          origin and 1 at x=0, y=-1; sequence gives the numbers on the
 *          positive x-axis.
 * https://oeis.org/A033953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = A033307(4*n^2 + 3*n) for n>0, a(0)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   digit_at_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_at_index.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute index in the digit stream */
    idx_t idx = 4 * n * n + 3 * n;   /* 4*n^2 + 3*n, fits in idx_t for n up to 10^9 */
    return digit_at_index(idx);
}

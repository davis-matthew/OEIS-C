/*
 * A179743  Triangle read by rows: antidiagonals of an array formed by
 *          sequences of the form a(0)=1, a(1) = (n+1); a(n+1), n>1 = 2*a(n).
 * https://oeis.org/A179743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading antidiagonals of the infinite array
 *            * A where * A[i][0] = A[0][j] = 1, * A[i][j] = (i+1) * 2^(j-1)
 *            for i>0 and j>0. * The i,j pair for a given index n is found by:
 *            * d = floor((sqrt(8*n+1)-1)/2) // diagonal index * o = n -
 *            d*(d+1)/2 // offset inside diagonal * i = d -...
 * Range:     u8 n <= 52, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }
    /* offset inside diagonal */
    idx_t prev_tri = d * (d + 1) / 2;
    idx_t o = n - prev_tri;
    idx_t i = d - o;
    idx_t j = o;

    /* Handle borders */
    if (i == 0 || j == 0) {
        return (seq_t)1;
    }

    /* Compute (i+1) * 2^(j-1) with safe multiplication. */
    seq_t base = (seq_t)(i + 1);
    seq_t power = (seq_t)1;
    for (idx_t k = 0; k < j - 1; ++k) {
        power *= 2;
    }
    return base * power;
}

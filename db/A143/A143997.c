/*
 * A143997  Rectangular array by antidiagonals: label each unit square in the
 *          first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which (x,y) is congruent mod 4 to one of the
 *          following: (1,4), (2,2), (3,3), (4,1); then R(m,n) is the number
 *          of UNmarked squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A143997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = m*n - floor(m*n/4) where (m,n) are the rectangle
 *            dimensions that correspond to the n-th entry of the table read
 *            along antidiagonals (m decreasing)
 * Range:     u8 n <= 544, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 to avoid UB */

    /* Find the antidiagonal sum S such that
       (S-1)*S/2 >= n > (S-2)*(S-1)/2                           */
    double discr = sqrt(1.0 + 8.0 * (double)n);
    idx_t S = (idx_t)((1.0 + discr) / 2.0);
    if ((S-1) * S / 2 < n) ++S;  /* adjust because of truncation */

    idx_t prev = (S-2) * (S-1) / 2;   /* total terms before this diagonal */
    idx_t k = n - prev - 1;           /* 0‑based position inside diagonal */

    idx_t m = (S-1) - k;              /* m decreases along the diagonal     */
    idx_t n_val = S - m;              /* corresponding n                     */

    seq_t prod = (seq_t)m * (seq_t)n_val;
    return prod - (prod / 4);         /* floor(m*n/4) is integer division */
}

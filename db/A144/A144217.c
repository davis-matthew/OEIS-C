/*
 * A144217  Weight array of A144216: a rectangular array by antidiagonals.
 * https://oeis.org/A144217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7340, u16 n <= 7340, u32 n <= 7340, u64 n <= 7340,
 *            u128 n <= 7340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight array of A144216 rendered by antidiagonals.
   For index n (1‑based): find the antidiagonal s with
   s*(s-1)/2 >= n, then t = n - (s-1)*(s-2)/2.
   Set m = t and col = s - t.  The weight w(m,col) is
   w = (m==1? col-1 : (col==1 ? m-1 : 0)). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* unused (offset 1) */

    /* find minimal s such that s*(s-1)/2 >= n */
    idx_t s = 2; /* earliest diagonal (1,1) has s=2 */
    while ((s * (s - 1)) / 2 < n) ++s;

    /* previous diagonal total */
    idx_t prev_total = ((s - 1) * (s - 2)) / 2;
    idx_t t = n - prev_total;          /* 1‑based position within diagonal */
    idx_t m = t;                       /* row index */
    idx_t col = s - t;                 /* column index */

    seq_t w;
    if (m == 1) {
        w = (seq_t)(col - 1);
    } else if (col == 1) {
        w = (seq_t)(m - 1);
    } else {
        w = 0;
    }
    return w;
}

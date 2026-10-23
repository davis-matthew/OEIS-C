/*
 * A048882  A convolution triangle of numbers obtained from A034255.
 * https://oeis.org/A048882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 35, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A048882 – convolution triangle of A034255.
    Recurrence:  a(n+1,m) = 4*(4*n+m)*a(n,m)/(n+1) + m*a(n,m-1)/(n+1)   (n≥m≥1)
    Base:        a(1,1)=1,   a(n,0)=0,   a(n,m)=0 for n<m.
    Sequence a(k) is the row‑major traversal of the triangle, starting with a(0)=a(1,1). */

#include <stdlib.h>
#include <string.h>

/* Use the helper for integer square root if desired
   (not needed here because we use a linear search for the row). */
seq_t a(idx_t n)
{
    /* Determine the row r and column m that contain the n‑th term
       in row‑major order.  The first index of row r is triangular(r‑1). */
    idx_t r = 1;
    while (1) {
        __uint128_t t = (__uint128_t)r * (r + 1) / 2;
        if (t > n)
            break;
        ++r;
    }
    idx_t prev_tri = (idx_t)(((__uint128_t)(r - 1) * r) / 2);
    idx_t m = n - prev_tri + 1;   /* 1‑based column index within the row */

    /* Allocate two rows (previous and current).  Size r+1 is enough for all
       indices that may be accessed during the DP. */
    seq_t *prev = (seq_t *)calloc((size_t)r + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)r + 1, sizeof(seq_t));
    if (!prev || !cur) {         /* allocation failure – return 0 */
        free(prev); free(cur);
        return (seq_t)0;
    }

    /* Base row: a(1,1) = 1 */
    prev[1] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t row = 1; row < r; ++row) {
        memset(cur, 0, ((size_t)r + 1) * sizeof(seq_t));   /* cur[0] stays 0 */
        for (idx_t col = 1; col <= row + 1; ++col) {
            seq_t term1 = prev[col]   * (seq_t)(4 * (4 * row + col));
            seq_t term2 = prev[col-1] * (seq_t)col;
            cur[col]    = (term1 + term2) / (row + 1);
        }
        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[m];
    free(prev);
    free(cur);
    return result;
}

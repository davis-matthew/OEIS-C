/*
 * A105793  Expansion of e.g.f. (1 + y)^(1 + x).
 * https://oeis.org/A105793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = s(r,k) + r*s(r-1,k), with s(n,k) the
 *            signed Stirling numbers of the first kind
 * Range:     i8 n <= 29, i16 n <= 55, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   triangle_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/triangle_row.h"

/* Compute a(n) using only O(n) extra memory and O(n^2) arithmetic
   operations, which is enough for n ≤ 10000. */
seq_t a(idx_t n) {
    idx_t r = triangle_row(n);
    idx_t c = n - r * (r + 1) / 2;          /* column in row r */

    /* Allocate arrays for two consecutive rows of Stirling numbers */
    seq_t *prev = NULL;  /* row i-1 */
    seq_t *cur  = NULL;  /* row i   */
    idx_t  prev_len = 0;
    idx_t  cur_len  = 0;

    /* row 0: s(0,0) = 1 */
    prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    prev[0] = 1;
    prev_len = 1;

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        /* Ensure cur has space for i+1 elements */
        if (cur_len < i + 1) {
            free(cur);
            cur = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
            cur_len = i + 1;
        }

        cur[0] = 0;          /* s(i,0) = 0 for i > 0 */
        cur[i] = 1;          /* s(i,i) = 1 */

        for (idx_t k = 1; k < i; ++k) {
            /* s(i,k) = s(i-1,k-1) - (i-1) * s(i-1,k) */
            cur[k] = prev[k - 1] - ((seq_t)(i - 1) * prev[k]);
        }

        /* Swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        /* prev_len becomes size of new prev (i+1),
           cur_len stays as size of old prev (i) automatically */
    }

    /* After the loop: prev = row r, cur = row r-1 (or NULL if r==0) */
    seq_t t_val = prev[c];
    if (c < r && cur != NULL) {
        t_val += ((seq_t)r * cur[c]);   /* add r * s(r-1, c) */
    }

    free(prev);
    free(cur);
    return t_val;
}

/*
 * A091747  Generalized Stirling2 array (7,2).
 * https://oeis.org/A091747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generalized Stirling2 array (7,2) – row‑wise flattening
   a(1,2) = 1
   For 2 ≤ k ≤ 2·n  (n ≥ 2):
       a(n,k) = (5*(n-1)+k)*(5*(n-1)+k-1)*a(n-1,k)
              + 2*(5*(n-1)+k-1)*a(n-1,k-1)
              + a(n-1,k-2)
   The output sequence `a(idx)` is obtained by concatenating all rows;
   row n has 2·n−1 elements, so indices 1 … n² belong to rows 1 … n.  */

#include <stdlib.h>

/* --- integer square‑root (floor) for 64‑bit values ------------------- */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t bit = 1ULL << 62;   /* highest power of two <= sqrt(UINT64_MAX) */
    idx_t x   = 0;
    while (bit) {
        idx_t y = x | bit;
        if (y <= n / y)            /* avoid overflow in y*y          */
            x = y;
        bit >>= 1;
    }
    return x;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used for the sequence       */

    /* Determine row number r such that r² ≥ n > (r‑1)²  */
    idx_t r = floor_sqrt_u64(n);
    if ((seq_t)r * (seq_t)r < (seq_t)n) r++;

    /* Local k inside the determined row: a(r, k) with k = 2 … 2*r   */
    idx_t k = n - (r-1)*(r-1) + 1;   /* 1‑based offset in the row      */
    if (k > 2*r) return 0;          /* safety check                     */

    /* Allocate working arrays for one row (size 2*r+1, indices 0 … 2*r) */
    size_t const sz = 2*r + 1;
    seq_t *prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!prev || !curr) {          /* allocation failed */
        free(prev); free(curr);
        return 0;
    }

    /* Row 1: only a(1,2)=1 */
    prev[2] = 1;

    /* Build rows 2 … r  using the recurrence */
    for (idx_t row = 2; row <= r; ++row) {
        seq_t M = (seq_t)(row - 1) * 5;          /* 5·(n‑1)            */
        for (idx_t j = 2; j <= 2*row; ++j) {
            seq_t va = (j <= 2*(row-1)) ? prev[j]   : 0;
            seq_t vb = (j-1 <= 2*(row-1)) ? prev[j-1] : 0;
            seq_t vc = (j-2 <= 2*(row-1)) ? prev[j-2] : 0;

            seq_t term1 = (M + j) * (M + j - 1) * va;
            seq_t term2 = 2 * (M + j - 1) * vb;
            curr[j] = term1 + term2 + vc;
        }
        /* Prepare for next iteration: swap arrays               */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}

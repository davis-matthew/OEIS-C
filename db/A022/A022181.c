/*
 * A022181  Triangle of Gaussian binomial coefficients [ n,k ] for q = 17.
 * https://oeis.org/A022181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n + 1)-st element when the triangle T(n,k) is *
 *            written row‑by‑row, where * T(0,0) = 1, T(n,0) = T(n,n) = 1, *
 *            T(n,k) = T(n‑1,k‑1) + 17^k * T(n‑1,k) for 1 ≤ k < n
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 1183 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Find the row r and column k in the flattened triangle for index n. */
static inline void annk(idx_t n, idx_t *r, idx_t *k)
{
    idx_t row = 0;
    /* triangular number t(row) = row*(row+1)/2  */
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;
    *r = row;
    *k = n - row * (row + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    annk(n, &r, &k);

    /* Base case: row 0 consists of a single 1. */
    if (r == 0)
        return (seq_t)1;

    /* Allocate two rows: prev for T(row‑1,*), cur for T(row,*) . */
    /* Size r+1 is enough for any row up to r. */
    seq_t *prev = mem_new(seq_t, (size_t)(r + 1));
    seq_t *cur  = mem_new(seq_t, (size_t)(r + 1));
    if (!prev || !cur)
    {
        mem_free(prev);
        mem_free(cur);
        return (seq_t)0;   /* allocation failure – return 0 */
    }

    /* Row 1: 1 1 */
    prev[0] = (seq_t)1;
    prev[1] = (seq_t)1;

    /* Build rows 2 .. r. */
    for (idx_t row = 2; row <= r; ++row)
    {
        cur[0] = (seq_t)1;
        cur[row] = (seq_t)1;

        seq_t q_pow = (seq_t)17;          /* 17^1 */

        for (idx_t j = 1; j < row; ++j)
        {
            cur[j] = prev[j - 1] + q_pow * prev[j];
            q_pow *= (seq_t)17;           /* advance to 17^{j+1} */
        }

        /* Swap rows: next iteration uses this row as prev. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[k];

    mem_free(prev);
    mem_free(cur);
    return result;
}

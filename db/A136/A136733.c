/*
 * A136733  Square array, read by antidiagonals, where T(n,k) = T(n,k-1) +
 *          T(n-1,k+n) for n>0, k>0, such that T(n,0) = T(n-1,n) for n>0 with
 *          T(0,k)=1 for k>=0.
 * https://oeis.org/A136733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136733
   The sequence is defined by the square array T(n,k):
       T(0,k) = 1                                  (any k ≥ 0)
       T(n,0) = T(n‑1,n)                           (n > 0)
       T(n,k) = T(n,k‑1) + T(n‑1,k+n)               (n > 0, k > 0)
   a(n) is read by antidiagonals: a(n) = T(n',k') where
       n = triangular(d) + offset   (0 ≤ offset < d+1)
       n' = d – offset
       k' = offset
   (triangular(x) = x*(x+1)/2)
*/

#include <stdlib.h>

/* triangular number */
static inline idx_t triangular(idx_t x)
{
    return x * (x + 1) / 2;
}

/* dynamic table for memoization: memo[n][k] = T(n,k).  Zero means “not yet computed”. */
static seq_t **memo = NULL;           /* pointer to rows */
static size_t *col_cap = NULL;        /* capacity of each row (number of columns) */
static size_t rows = 0;               /* number of rows currently allocated */

/* ensure that row n exists and has capacity at least k+1 */
static void ensure_row_capacity(idx_t n, idx_t k)
{
    /* grow number of rows if necessary */
    if ((size_t)n >= rows) {
        size_t new_rows = n + 1;
        memo = (seq_t **)realloc(memo, new_rows * sizeof(seq_t *));
        col_cap = (size_t *)realloc(col_cap, new_rows * sizeof(size_t));
        for (size_t i = rows; i < new_rows; ++i) {
            memo[i] = NULL;
            col_cap[i] = 0;
        }
        rows = new_rows;
    }

    /* grow column capacity of row n if necessary */
    if ((size_t)k >= col_cap[(size_t)n]) {
        size_t cur = col_cap[(size_t)n];
        size_t needed = k + 1;
        if (cur == 0) cur = 1;
        while (cur < needed) cur <<= 1;   /* double until enough */
        memo[(size_t)n] = (seq_t *)realloc(memo[(size_t)n], cur * sizeof(seq_t));
        /* initialise new entries to 0 (sentinel for “not computed”) */
        for (size_t j = col_cap[(size_t)n]; j < cur; ++j)
            memo[(size_t)n][j] = (seq_t)0;
        col_cap[(size_t)n] = cur;
    }
}

/* recursive evaluation of T(n,k) with memoization */
static seq_t T(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;          /* base row */

    ensure_row_capacity(n, k);
    if (memo[(size_t)n][(size_t)k] != (seq_t)0)
        return memo[(size_t)n][(size_t)k];

    seq_t val;
    if (k == 0) {
        /* T(n,0) = T(n-1,n) */
        val = T(n - 1, n);
    } else {
        seq_t a = T(n, k - 1);
        seq_t b = T(n - 1, k + n);
        val = a + b;
    }

    memo[(size_t)n][(size_t)k] = val;
    return val;
}

/* required function */
seq_t a(idx_t n)
{
    /* find d such that triangular(d) ≤ n < triangular(d+1) */
    idx_t d = 0;
    while (triangular(d + 1) <= n) ++d;

    idx_t offset = n - triangular(d);
    idx_t nprime = d - offset;   /* row index */
    idx_t kprime = offset;       /* column index */

    return T(nprime, kprime);
}

/*
 * A172346  Triangle t(n,k) read by rows: fibonomial ratios c(n)/(c(k)*c(n-k))
 *          where c are partial products of a generalized Fibonacci sequence
 *          with multiplier m=8.
 * https://oeis.org/A172346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is the (n‑th) term of the flattened triangle t(r,k) where
 *            * r is the row index and k the column in that row: * * r = max{m
 *            | m(m+1)/2 <= n} * k = n - r(r+1)/2 * * t(r,k) = c(r) / (c(k) *
 *            c(r-k)), (t(r,0)=t(r,r)=1) * * c(n) = ∏_{i=1..n} f(i) with *
 *            f(1) = 1, f(2) = 8, * f(i) = 8*f(i-1) + f(...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Determine row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((__int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;   /* column in row r */

    if (r == 0)                      /* a(0) = 1 */
        return (seq_t)1;

    /* Allocate temporary arrays */
    size_t sz = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *f    = (seq_t *)malloc((sz + 1) * sizeof(seq_t));  /* f[1..r] */

    /* Compute the Fibonacci-like sequence f[1..r] */
    f[1] = (seq_t)1;
    if (r >= 2) {
        f[2] = (seq_t)8;
        for (idx_t i = 3; i <= r; ++i)
            f[i] = f[i - 1] * (seq_t)8 + f[i - 2];
    }

    /* Initialize the first row (only element 1) */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r in place */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)1;
        curr[i] = (seq_t)1;
        for (idx_t j = 1; j < i; ++j)
            curr[j] = (prev[j - 1] * f[i]) / f[j];
        /* copy current row to prev for next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = curr[j];
    }

    seq_t result = prev[k];   /* a(n) = t(r,k) */

    free(prev);
    free(curr);
    free(f);
    return result;
}

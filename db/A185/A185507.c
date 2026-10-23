/*
 * A185507  Second accumulation array, T, of the natural number array A000027,
 *          by antidiagonals.
 * https://oeis.org/A185507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = k*n*(k+1)*(n+1)*(3*n^2 +
 *            (4*k+11)*n + 3*k^2 - k + 16)/144. The (row,col) pair is obtained
 *            from the antidiagonal representation of index n with 1‑based
 *            coordinates
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the accumulation formula T(n,k). */
static seq_t T_func(idx_t n, idx_t k)
{
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;
    seq_t num   = kk * nn * (kk + 1) * (nn + 1);
    seq_t inner = 3 * nn * nn + (4 * kk + 11) * nn
                + 3 * kk * kk - kk + 16;
    num *= inner;
    num /= 144;                /* division is exact by formula definition */
    return num;
}

/* Find the diagonal d (sum of row and column) that contains position m. */
static idx_t diagonal_of(idx_t m)
{
    idx_t d = 1;
    while ( (d * (d - 1ULL)) / 2 < m )
        ++d;
    return d;
}

/* a(n) for 1‑based index n. */
seq_t a(idx_t n)
{
    /* Find the diagonal the nth element lies on. */
    idx_t d        = diagonal_of(n);                   /* d = row + col    */
    idx_t prev     = (d - 2ULL) * (d - 1ULL) / 2;      /* elements before */
    idx_t offset   = n - prev;                        /* 1 … d-1         */
    idx_t row      = offset;                          /* row index        */
    idx_t col      = d - offset;                      /* column index     */
    return T_func(row, col);
}

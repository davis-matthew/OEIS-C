/*
 * A216817  Variant of Lucas triangle: see Comments for definition.
 * https://oeis.org/A216817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, tabf
 * Formula:   a(n) reads the columns c = 0,1,2,... of a shifted copy of the
 *            square array A(i,j) = T(i+j,j) of A029635 from the bottom up. T
 *            is the (1,2)-Pascal triangle: T(0,0)=2, T(m,0)=1 for m>0, and
 *            T(m,k)=C(m,k)+C(m-1,k-1). Row i of the array starts at column
 *            s(i) = 2*i + floor(i/4). Column c lists A(i, c-...
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A216817_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

static seq_t A216817_T(idx_t r, idx_t k)
{
    if (r == 0) return (seq_t)2;
    if (k == 0) return (seq_t)1;
    return A216817_binom(r, k) + A216817_binom(r - 1, k - 1);
}

static idx_t A216817_shift(idx_t i)
{
    return 2 * i + i / 4;
}

seq_t a(idx_t n)
{
    idx_t start = 0;
    idx_t c = 0;
    for (;;) {
        /* number of array rows present in column c */
        idx_t m = 0;
        while (A216817_shift(m) <= c) ++m;
        if (n < start + m) {
            idx_t p = n - start;
            idx_t i = m - 1 - p;
            idx_t j = c - A216817_shift(i);
            return A216817_T(i + j, j);
        }
        start += m;
        ++c;
    }
}

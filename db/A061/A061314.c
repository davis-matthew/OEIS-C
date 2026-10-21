/*
 * A061314  Table read by descending antidiagonals where T(n,k) = T(n,k-1) +
 *          T(n,k-1)^2/k^2 and T(n,0)=n.
 * https://oeis.org/A061314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where * T(n,0) = n * T(n,k) = T(n,k-1) +
 *            (T(n,k-1)/k)^2 * (i,j) are the coordinates on the descending
 *            anti‑diagonal: * let S be the smallest integer with (S+1)(S+2)/2
 *            > n, * let t = n - S(S+1)/2, * then i = t, j = S - t. * Sequence
 *            is read by traversing the table T(n,k) along these...
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 47, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   row_col_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrt */
#include "../utils/row_col_value.h"

static idx_t
find_diagonal(idx_t n)
{
    /* Find the smallest S such that (S+1)(S+2)/2 > n. */
    long double s = sqrt((long double)(8.0L * n + 1.0L));
    idx_t S = (idx_t)((s - 3.0L) / 2.0L);
    if (S > n) S = n;

    /* Adjust upward until the inequality holds. */
    unsigned __int128 val = (unsigned __int128)(S + 1) * (S + 2) / 2;
    while (val <= n) {
        ++S;
        val = (unsigned __int128)(S + 1) * (S + 2) / 2;
    }
    /* Adjust downward if we overshot. */
    while ((unsigned __int128)S * (S + 1) / 2 > n) {
        --S;
    }
    return S;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n==0 quickly. */
    if (n == 0) return (seq_t)0;

    /* Determine the diagonal and the position within it. */
    idx_t S = find_diagonal(n);
    unsigned __int128 before = (unsigned __int128)S * (S + 1) / 2;
    idx_t offset = n - (idx_t)before;   /* offset within diagonal */
    idx_t row = offset;
    idx_t col = S - offset;

    return row_col_value(row, col);
}

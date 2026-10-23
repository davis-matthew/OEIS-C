/*
 * A105197  Triangle, read by rows, of Stirling numbers of second kind,
 *          S2(n,k), multiplied by k^k, for n >= 1, 1<=k<=n.
 * https://oeis.org/A105197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(i,j) = Stirling2(i,j) * j^j, read by rows as defined in OEIS
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   power_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/power_seq.h"

/* Locate the row r such that r(r+1)/2 >= n  (n >= 1) */
static idx_t
find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cum = (seq_t)mid * ((seq_t)mid + 1u) / 2u;
        if (cum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1; return 0 */

    /* Recover (row, column) from the flat index n */
    idx_t row = find_row(n);
    seq_t prev = (seq_t)(row - 1u) * (seq_t)row / 2u;   /* cumulative count before this row */
    idx_t col = n - (idx_t)prev;                       /* 1‑based column index */

    /* Compute Stirling numbers of the second kind S(row,col) */
    size_t sz = (size_t)col + 1u;          /* need indices 0..col */
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return 0;                          /* allocation failure */

    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;                              /* S(0,0)=1 */

    for (idx_t r = 1; r <= row; ++r) {
        idx_t m = (r < col) ? r : col;      /* only up to column limit */
        for (idx_t j = m; j >= 1; --j) {
            dp[j] = (seq_t)j * dp[j] + dp[j - 1];
        }
        dp[0] = 0;                          /* S(r,0)=0 for r>0 */
    }

    seq_t stir = dp[col];
    free(dp);

    /* Compute col^col */
    seq_t powcol = power_seq((seq_t)col, col);

    return stir * powcol;
}

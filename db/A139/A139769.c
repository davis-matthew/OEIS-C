/*
 * A139769  T(n,k) = [x^k] Product_{m=1..n} d/dx Sum_{i=1..m} x^i; triangle
 *          read by rows, n >= 0, 0 <= k <= A161680(n).
 * https://oeis.org/A139769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 153, u64 n <= 529,
 *            u128 n <= 2184, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n)  is the n‑th term of the flattened triangle
 *  T(n,k) = [x^k] ∏_{m=1..n} (∑_{i=1..m} x^i)' ,
 *  where (∑_{i=1..m} x^i)' = ∑_{j=0..m-1} (j+1) x^j .
 *  The triangle is read row by row; this function computes the
 *  coefficient of the polynomial after processing the appropriate
 *  row.  It is O(r^3) where r≈√(2n); for the required n≤10000
 *  this is negligible. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* --- determine row r and column k from the flattened index n ----- */
    idx_t r = 0;
    idx_t start = 0;          /* linear index where current row starts */
    for (;; ++r)
    {
        idx_t len = (r * (r - 1)) / 2 + 1;   /* length of current row */
        if (n < start + len) break;          /* n lies in this row   */
        start += len;                        /* move to next row     */
    }
    idx_t k = n - start;  /* column index within the row */

    /* --- build the polynomial incrementally up to row r ---------------- */
    seq_t *dp   = (seq_t*)calloc(1, sizeof(seq_t));   /* current polynomial */
    if (!dp) return 0;  /* allocation failure; should not happen */
    dp[0] = (seq_t)1;
    size_t cur_len = 1;

    for (idx_t m = 1; m <= r; ++m)
    {
        size_t glen   = (size_t)m;               /* length of factor g_m */
        size_t new_len = cur_len + glen - 1;     /* new polynomial length */

        seq_t *tmp = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (!tmp) { free(dp); return 0; }

        for (size_t i = 0; i < cur_len; ++i)
        {
            seq_t val = dp[i];
            if (!val) continue;
            for (size_t j = 0; j < glen; ++j)
            {
                tmp[i + j] += val * ( (seq_t)(j + 1) );
            }
        }

        free(dp);
        dp     = tmp;
        cur_len = new_len;
    }

    /* dp[k] now holds the desired coefficient */
    seq_t result = dp[(size_t)k];
    free(dp);
    return result;
}

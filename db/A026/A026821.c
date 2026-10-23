/*
 * A026821  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into distinct parts, the least being k, for k=1,2,...,n.
 * https://oeis.org/A026821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangular array T(n,k) * where
 *            T(n,k) counts partitions of n into distinct parts with *
 *            smallest part k. The array is read row by row, rows numbered *
 *            from 1; the first term is T(1,1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* -------------  DP storage ------------- */
static seq_t **dp = NULL;           /* dp[row][k] = T(row,k)   */
static seq_t **pref = NULL;         /* pref[row][k] = sum_{t=1}^{k} dp[row][t] */
static idx_t dp_rows = 0;

/* -------------  ensure tables up to 'rows' ------------- */
static void ensure_rows(idx_t rows)
{
    if (rows <= dp_rows) return;

    /* grow the array of pointers */
    seq_t **new_dp    = (seq_t**)realloc(dp,    (rows + 1) * sizeof(seq_t*));
    seq_t **new_pref  = (seq_t**)realloc(pref,  (rows + 1) * sizeof(seq_t*));
    if (!new_dp || !new_pref) return;        /* allocation failed — ignore */
    dp    = new_dp;
    pref  = new_pref;

    for (idx_t r = dp_rows + 1; r <= rows; ++r)
    {
        /* allocate two rows of size r+1 (index 0 unused) */
        dp[r]    = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
        pref[r]  = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!dp[r] || !pref[r]) continue; /* ignore failure */

        /* compute dp[r][k] for k from r down to 1 */
        for (idx_t k = r; k >= 1; --k)
        {
            if (k == r)
            {
                dp[r][k] = (seq_t)1;
            }
            else
            {
                idx_t L = k + 1;            /* lower bound of sum range */
                idx_t R = r - k;            /* upper bound of sum range */

                if (L > R)
                {
                    dp[r][k] = (seq_t)0;
                }
                else
                {
                    /* use prefix sums of previous row r-k */
                    seq_t sum   = pref[r - k][R];
                    seq_t sub   = pref[r - k][L - 1];
                    dp[r][k]    = sum - sub;  /* prefix[R] - prefix[L-1] */
                }
            }
        }

        /* compute prefix sums for this row */
        pref[r][0] = (seq_t)0;
        for (idx_t j = 1; j <= r; ++j)
        {
            pref[r][j] = pref[r][j - 1] + dp[r][j];
        }
    }

    dp_rows = rows;
}

/* -------------  main sequence function ------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is 1‑indexed */

    /* find row r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);
    if ((r * (r + 1)) / 2 < n) ++r;
    while ((r > 1) && (((r - 1) * r) / 2) >= n) --r;

    /* column position in this row */
    idx_t col = n - ((r - 1) * r) / 2;

    /* build tables up to the required row */
    ensure_rows(r);

    /* return the desired entry */
    return dp[r][col];
}

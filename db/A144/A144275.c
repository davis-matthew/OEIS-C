/*
 * A144275  Lower triangular array called S2hat(-2) related to partition
 *          number array A144274.
 * https://oeis.org/A144275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A144275.
   Definition:
   Let f(k) = |S2(-2;k,1)| = product_{t=2..k} (3*t-4),   f(1)=1.
   Then a(n,m) = sum over all partitions of n into exactly m parts of 
   the product of f(parts).  The sequence a(k) is the
   row‑wise concatenation of the triangular array a(n,m) for
   n = 1,2,3,… and m = 1,…,n.
*/

#include <stdint.h>
#include <stdlib.h>

static seq_t *dp_table = NULL;         /* flattened dp[n][m] */
static idx_t  dp_rows   = 0;           /* number of rows currently computed  */
static seq_t *f_table   = NULL;        /* f[k] weights   */
static idx_t  f_rows    = 0;           /* number of f values computed */

/* Ensure dp_table and f_table have capacity for rows up to 'max_rows' (inclusive). */
static void ensure_dp(idx_t max_rows)
{
    if (dp_rows >= max_rows) return;

    /* allocate/resize f_table */
    seq_t *new_f = (seq_t*)realloc(f_table, (max_rows + 1) * sizeof(seq_t));
    if (!new_f) exit(1);
    f_table = new_f;

    /* compute f(k) for k = 1..max_rows */
    f_table[1] = (seq_t)1;
    for (idx_t k = 2; k <= max_rows; ++k)
    {
        f_table[k] = (seq_t)(3 * k - 4) * f_table[k - 1];
    }

    /* allocate/build dp_table */
    size_t sz = (size_t)(max_rows + 1) * (max_rows + 1);
    seq_t *new_dp = (seq_t*)realloc(dp_table, sz * sizeof(seq_t));
    if (!new_dp) exit(1);
    dp_table = new_dp;

    /* initialise to zero */
    for (size_t i = 0; i < sz; ++i) dp_table[i] = (seq_t)0;
    /* base case */
    dp_table[0] = (seq_t)1;                   /* dp[0][0] */

    /* dynamic programming: unordered partitions with weights f(k) */
    for (idx_t k = 1; k <= max_rows; ++k)
    {
        seq_t val = f_table[k];
        for (idx_t n = k; n <= max_rows; ++n)
        {
            for (idx_t m = 1; m <= n; ++m)
            {
                seq_t *dp_nm   = &dp_table[n * (max_rows + 1) + m];
                seq_t *dp_prev = &dp_table[(n - k) * (max_rows + 1) + (m - 1)];
                *dp_nm += val * (*dp_prev);
            }
        }
    }

    dp_rows = max_rows;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but guard anyway */

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* column within that row */
    idx_t prev_total = (r * (r - 1)) / 2;
    idx_t col = n - prev_total;     /* 1-indexed */

    /* compute dp up to row r if not already done */
    ensure_dp(r);

    /* return the stored value */
    return dp_table[r * (dp_rows + 1) + col];
}

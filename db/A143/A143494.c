/*
 * A143494  Triangle read by rows: 2-Stirling numbers of the second kind.
 * https://oeis.org/A143494
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑Stirling numbers of the second kind (triangle read by rows).
    Recurrence:  T(2,2) = 1,   T(n,1)=T(1,n)=0
                  T(n,k) = T(n-1,k-1) + k*T(n-1,k)   for n>2 , k>1
    For this code a(n) is the n‑th term of the flattened triangle
    (a(2)=1, a(3)=2, a(4)=1, …).  */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined below the offset */

    /* zero–based position in the flattened triangle */
    idx_t pos = n - 2;

    /* Find the row r (≥2) such that pos < r*(r-1)/2.  */
    idx_t r = 2;
    idx_t cumulative = 1;           /* r*(r-1)/2 for r=2 */
    while (cumulative <= pos) {
        ++r;
        cumulative += r - 1;
    }

    /* Position inside the found row */
    idx_t prev_cumulative = cumulative - (r - 1);   /* up to row r‑1 */
    idx_t offset = pos - prev_cumulative;           /* 0‑based inside row r */
    idx_t k = offset + 2;                           /* column index */

    /* Static storage for the triangle.
       dp[i] points to an array of length i+1; indices 0 and 1 are unused. */
    static seq_t **dp = NULL;
    static size_t rows_allocated = 0;
    static size_t rows_computed  = 0;

    if ((size_t)r > rows_computed) {
        /* Ensure storage for rows 0…r */
        size_t needed = (size_t)r + 1;
        if (needed > rows_allocated) {
            dp = (seq_t **)realloc(dp, needed * sizeof(seq_t *));
            for (size_t i = rows_allocated; i < needed; ++i) dp[i] = NULL;
            rows_allocated = needed;
        }

        /* Compute new rows one by one */
        for (size_t rr = rows_computed + 1; rr <= (size_t)r; ++rr) {
            /* Allocate array for row rr (length rr+1) */
            seq_t *row = (seq_t *)malloc((rr + 1) * sizeof(seq_t));
            /* initialise to zero */
            for (size_t i = 0; i <= rr; ++i) row[i] = 0;

            if (rr == 2) { /* base case */
                row[2] = 1;
            } else {
                for (idx_t kk = 2; kk <= rr; ++kk) {
                    seq_t left   = (kk-1 <= (idx_t)(rr-1)) ? dp[rr-1][kk-1] : 0;
                    seq_t down   = (kk   <= (idx_t)(rr-1)) ? dp[rr-1][kk]   : 0;
                    row[kk] = left + (seq_t)kk * down;
                }
            }
            dp[rr] = row;
        }
        rows_computed = (size_t)r;
    }

    return dp[(size_t)r][(size_t)k];
}

/*
 * A180562  Triangle read by rows: T(n,k) = number of binary words of length n
 *          avoiding 010 and having k 1's.
 * https://oeis.org/A180562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 287, u32 n <= 970, u64 n <= 3533,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180562: read a triangle T(n,k) by rows.
   Observation: the sequence is obtained by concatenating the rows of
   the triangle T(n,k) defined by
        T(n,k) = T(n-1,k) + T(n-1,k-1) - T(n-2,k-1) + T(n-3,k-2)
   with initial values
        T(0,0)=1,
        T(1,0)=T(1,1)=1,
        T(2,0)=T(2,2)=1,  T(2,1)=2,
   and T(n,k)=0 for k<0 or k>n.
   For a given index n let
        r = floor( (sqrt(8n+1)-1)/2 )
   be the row number (0‑based) such that
        r(r+1)/2 <= n < (r+1)(r+2)/2,
   and let
        c = n - r(r+1)/2
   be the column within that row.  Then a(n) = T(r,c).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* find the row r and column c that correspond to the flattened
       index n.  r is the largest integer with r(r+1)/2 <= n. */
    idx_t r = 0;
    while (true) {
        idx_t next_start = (r + 1) * (r + 2) / 2;
        if (next_start > n)
            break;
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    /* trivial case r==0 */
    if (r == 0)
        return (seq_t)1;

    /* allocate a small 2‑D array of size (r+1) × (r+1)   */
    size_t rows  = (size_t)(r + 1);
    size_t total = rows * rows;
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* fallback, should not happen */

    /* macro for indexing dp[n][k] */
    #define DP(n,k) dp[(size_t)(n)*rows + (size_t)(k)]

    /* initialize to zero */
    for (size_t i = 0; i < total; ++i)
        dp[i] = (seq_t)0;

    /* base values */
    DP(0,0) = (seq_t)1;
    if (r >= 1) {
        DP(1,0) = (seq_t)1;
        DP(1,1) = (seq_t)1;
    }
    if (r >= 2) {
        DP(2,0) = (seq_t)1;
        DP(2,1) = (seq_t)2;
        DP(2,2) = (seq_t)1;
    }

    /* build rows up to r using the recurrence */
    for (idx_t nrow = 3; nrow <= r; ++nrow) {
        for (idx_t k = 0; k <= nrow; ++k) {
            seq_t sum = (seq_t)0;
            if (k <= nrow - 1)
                sum += DP(nrow - 1, k);
            if (k >= 1)
                sum += DP(nrow - 1, k - 1);
            if (k >= 2)
                sum += DP(nrow - 3, k - 2);
            if (k >= 1)
                sum -= DP(nrow - 2, k - 1);
            DP(nrow, k) = sum;
        }
    }

    seq_t result = DP(r, c);

    free(dp);
    #undef DP
    return result;
}

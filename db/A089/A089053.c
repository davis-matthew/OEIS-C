/*
 * A089053  Triangle read by rows: T(n,k) (n >= 1, 1 <= k <= n) = number of
 *          partitions of n into exactly k powers of 2.
 * https://oeis.org/A089053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A089053 (triangle read by rows).
   a(n) is the element obtained by concatenating all rows of the
   triangle T(n,k): number of partitions of n into exactly k powers
   of two.  The algorithm uses a dynamic programming table
   dp[s][k] = T(s,k), computed for all 0 ≤ s ≤ required row.
   Complexity is O(R² log R) where R is the row index needed for
   the current n.  The table is stored in static variables and
   expanded only when a larger row is requested.  All arithmetic
   stays inside the seq_t type and requires only integer operations. */

#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>

/* forward declaration of the required function */
seq_t a(idx_t n);

/* static table of DP values: dp[s] points to an array of size s+1 */
static seq_t **dp = NULL;          /* dp[s][k] for 0 ≤ k ≤ s */
static idx_t dp_rows = 0;          /* number of rows currently stored (0 if none) */

/* free the entire DP table */
static void free_dp(void) {
    if (dp == NULL) return;
    for (idx_t i = 0; i < dp_rows; ++i) {
        free(dp[i]);
    }
    free(dp);
    dp = NULL;
    dp_rows = 0;
}

/* build the DP table up to row 'upto' (inclusive) */
static void compute_dp(idx_t upto) {
    free_dp();                                     /* discard old table */
    dp = (seq_t**)malloc((size_t)(upto + 1) * sizeof(seq_t*));
    dp_rows = upto + 1;
    for (idx_t s = 0; s <= upto; ++s) {
        dp[s] = (seq_t*)calloc((size_t)(s + 1), sizeof(seq_t));
    }
    dp[0][0] = (seq_t)1;                     /* base case: empty sum, 0 parts */

    /* iterate over all powers of two up to 'upto' */
    for (idx_t power = 1; power <= upto; power <<= 1) {
        for (idx_t s = power; s <= upto; ++s) {
            seq_t *dst = dp[s];
            seq_t *src = dp[s - power];
            idx_t maxk = s - power + 1;      /* largest k for which src[k-1] exists */
            for (idx_t k = 1; k <= maxk; ++k) {
                dst[k] += src[k - 1];
            }
        }
    }
}

/* ensure that the DP table contains at least rows 0..needed */
static void ensure_dp(idx_t needed) {
    if (needed + 1 <= dp_rows) return;      /* already available */
    compute_dp(needed);
}

/* convert global index n (0‑based) to row number r (1‑based) */
static idx_t row_of(idx_t n) {
    /* r = floor((sqrt(8*n+1)+1)/2) */
    double dn = (double)n;
    return (idx_t)((sqrt(8.0 * dn + 1.0) + 1.0) / 2.0);
}

/* main sequence function */
seq_t a(idx_t n) {
    /* Determine the triangle row and column for the global index n. */
    idx_t r = row_of(n);                         /* 1‑based row number */
    ensure_dp(r);                                /* make sure dp up to r is ready */

    idx_t prev = (r - 1) * r / 2;                /* number of elements before row r */
    idx_t k = n - prev + 1;                      /* 1‑based column in row r */

    return dp[r][k];
}

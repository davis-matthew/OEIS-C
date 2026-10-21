/*
 * A072705  Triangle of number of unimodal compositions of n into exactly k
 *          distinct terms.
 * https://oeis.org/A072705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = T(r,k), where T(r,k) is the number of unimodal
 *            compositions of r into exactly k distinct terms. The index n
 *            refers to the flattened triangle read row‑wise, so we first
 *            determine the row r and column k: r = smallest integer with
 *            r*(r+1)/2 >= n k = n - (r*(r-1)/2) Then T(r,k) is computed as...
 * Range:     u8 n <= 234, u16 n <= 1491, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(seq_t base, idx_t exp) */

#define MAX_ROW 200   /* maximum row needed for index up to ~20000 */

/* DP table: dp[n][k] = number of partitions of n into exactly k distinct parts */
static seq_t dp[MAX_ROW + 1][MAX_ROW + 1];
static bool dp_initialized = false;

/* One‑time initialization of the DP table */
static void init_dp(void)
{
    if (dp_initialized) return;
    for (idx_t i = 0; i <= MAX_ROW; ++i)
        for (idx_t j = 0; j <= MAX_ROW; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;   /* empty partition */

    for (idx_t part = 1; part <= MAX_ROW; ++part) {
        for (idx_t sum = MAX_ROW; sum >= part; --sum) {
            for (idx_t parts = 1; parts <= sum; ++parts) {
                dp[sum][parts] += dp[sum - part][parts - 1];
            }
        }
    }
    dp_initialized = true;
}

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;          /* sum of first r terms */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Compute column k */
    idx_t prev_cumulative = cumulative - r;  /* sum of terms up to row r-1 */
    idx_t k = n - prev_cumulative;          /* 1‑based column index */

    /* Guard against out‑of‑range access (unlikely for allowed n) */
    if (r > MAX_ROW || k > r) {
        /* If needed, enlarge MAX_ROW and re‑initialize.  For current
           constraints this situation will not occur. */
        return 0;
    }

    /* Ensure DP table is ready */
    init_dp();

    /* Retrieve the number of partitions into distinct parts */
    seq_t partitions = dp[r][k];

    if (partitions == 0) return 0;

    /* Compute 2^(k-1) and multiply */
    seq_t two_pow = pow_uint(2, k - 1);
    return partitions * two_pow;
}

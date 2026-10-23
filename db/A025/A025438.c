/*
 * A025438  Number of partitions of n into 5 distinct squares.
 * https://oeis.org/A025438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 5 distinct squares
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* integer square root, returns floor(sqrt(n)) */
static inline size_t isqrt(size_t n)
{
    size_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* DP tables: dp[k][s] = ways to reach sum s using exactly k distinct squares
   Tables are allocated only when a larger n is requested. */
static seq_t **dp = NULL;
static size_t cur_max_n = 0;

static void ensure_dp(size_t n)
{
    if (dp != NULL && n <= cur_max_n)
        return;                    /* already big enough */

    /* free old tables if they exist */
    if (dp != NULL) {
        for (int k = 0; k <= 5; ++k)
            free(dp[k]);
        free(dp);
    }

    /* allocate new tables, one entry per possible sum 0..n */
    dp = (seq_t **)malloc(6 * sizeof(seq_t *));
    for (int k = 0; k <= 5; ++k)
        dp[k] = (seq_t *)calloc(n + 1, sizeof(seq_t));

    cur_max_n = n;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    ensure_dp(N);

    /* reset tables for the new test case */
    for (int k = 0; k <= 5; ++k)
        memset(dp[k], 0, (N + 1) * sizeof(seq_t));

    dp[0][0] = 1;                    /* one way to form sum 0 with 0 squares */

    size_t max_i = isqrt(N);

    /* we must include the square 0, since 0^2=0 is allowed once */
    for (size_t i = 0; i <= max_i; ++i) {
        seq_t val = (seq_t)i * (seq_t)i;   /* square value */
        if (val > (seq_t)N)                /* should not happen for i <= max_i */
            break;
        size_t v = (size_t)val;            /* safe: val <= N */

        for (int k = 5; k >= 1; --k) {
            for (size_t s = N - v; ; --s) {
                if (dp[k - 1][s])
                    dp[k][s + v] += dp[k - 1][s];
                if (s == 0)
                    break;
            }
        }
    }

    return dp[5][N];
}

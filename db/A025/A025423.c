/*
 * A025423  Number of partitions of n into 8 squares.
 * https://oeis.org/A025423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k=0}^{∞} x^{k^2})^8
 * Range:     u8 n <= 229, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Count of partitions of n into exactly 8 squares (0 allowed). */

#include <stdlib.h>

static seq_t *dp[9];          /* dp[k][s] = number of ways to write s as sum of k non‑zero squares */
static idx_t max_n = (idx_t)-1; /* largest n for which tables are built */

/* Build tables up to N */
static void recompute(idx_t N)
{
    /* Free any previous tables */
    for (int k = 0; k < 9; ++k) {
        free(dp[k]);
        dp[k] = NULL;
    }

    /* Allocate new tables, zeroed */
    size_t len = (size_t)(N + 1);
    for (int k = 0; k < 9; ++k) {
        dp[k] = (seq_t*)calloc(len, sizeof(seq_t));
        if (!dp[k]) return;   /* abort on allocation failure */
    }

    /* Base case: empty sum (0 squares) */
    dp[0][0] = (seq_t)1;

    /* Coin‑change DP: use each non‑zero square as an unlimited coin */
    for (idx_t sq = 1; sq * sq <= N; ++sq) {
        idx_t val = sq * sq;
        for (idx_t s = val; s <= N; ++s) {
            size_t prev = (size_t)(s - val);
            for (int k = 1; k <= 8; ++k) {
                dp[k][(size_t)s] += dp[k-1][prev];
            }
        }
    }

    max_n = N;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n > max_n || max_n == (idx_t)-1) recompute(n);

    seq_t total = 0;
    size_t idx = (size_t)n;
    for (int k = 0; k <= 8; ++k) {
        total += dp[k][idx];
    }
    return total;
}

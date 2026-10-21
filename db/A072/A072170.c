/*
 * A072170  Square array T(n,k) (n >= 0, k >= 2) read by antidiagonals, where
 *          T(n,k) is the number of ways of writing n as Sum_{i>=0} c_i 2^i,
 *          c_i in {0,1,...,k-1}.
 * https://oeis.org/A072170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1389, u16 n <= 9875, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via bounded coin-change DP (T(n,k) read by antidiagonals). */

#include <stdbool.h>

#define MAXN 150     /* maximum n needed for indices up to 10000 */
#define MAXK 150     /* maximum k needed (k <= n+2 for n<=MAXN) */

static seq_t T[MAXN + 1][MAXK + 1];

/* Build the table T[n][k] for 0 <= n <= MAXN, 2 <= k <= MAXK. */
static void init_table(void) {
    static bool computed = false;
    if (computed) return;
    computed = true;

    for (int k = 2; k <= MAXK; ++k) {
        seq_t dp[MAXN + 1];
        seq_t ndp[MAXN + 1];
        for (int i = 0; i <= MAXN; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        for (int p = 1; p <= MAXN; p <<= 1) {
            for (int i = 0; i <= MAXN; ++i) ndp[i] = (seq_t)0;

            for (int offset = 0; offset < p; ++offset) {
                seq_t window = (seq_t)0;
                for (int t = 0, s = offset; s <= MAXN; ++t, s += p) {
                    window += dp[s];
                    if (t >= k) window -= dp[s - k * p];
                    ndp[s] = window;
                }
            }
            /* copy ndp back to dp */
            for (int i = 0; i <= MAXN; ++i) dp[i] = ndp[i];
        }

        for (int n = 0; n <= MAXN; ++n) {
            T[n][k] = dp[n];
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_table();

    /* Find diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;

    idx_t base = s * (s + 1) / 2;   /* triangular number for diagonal s */
    idx_t pos   = n - base;         /* position within the diagonal */
    idx_t n_val = s - pos;          /* row index n in T */
    idx_t k_val = pos + 2;          /* column index k in T (k >= 2) */

    /* Bounds are safe for n <= 10000: n_val <= MAXN, k_val <= MAXK */
    return T[(int)n_val][(int)k_val];
}

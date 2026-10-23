/*
 * A189898  Triangular array read by rows. T(n,k) is the number of digraphs
 *          with n labeled nodes having exactly k undirected (or weak)
 *          components, n >= 1, 1 <= k <= n.
 * https://oeis.org/A189898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = sum_{s=1}^{n-k+1} binom(n-1,s-1) * f(s) * T(n-s,k-1),
 *            where f(s) is the number of digraphs on s labeled vertices whose
 *            underlying undirected graph is connected. a(idx) is the
 *            triangular array read by rows: a(1)=T(1,1), a(2)=T(2,1),
 *            a(3)=T(2,2), a(4)=T(3,1), etc
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, decompose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/decompose.h"

/* Max row needed for index ≤ 10000: r*(r+1)/2 ≥ 10000 -> r ≈ 140.
   We use a generous bound. */
#define MAX_N 200

static bool initialized = false;
static seq_t dp[MAX_N+1][MAX_N+1]; /* dp[n][k] */
static seq_t f[MAX_N+1];           /* f[n] = dp[n][1] */
static seq_t binom_tbl[MAX_N+1][MAX_N+1];

/* Compute binomial coefficients binom_tbl[n][k] for 0 ≤ n,k ≤ MAX_N. */
static void init_binoms(void)
{
    for (idx_t n = 0; n <= MAX_N; ++n) {
        binom_tbl[n][0] = 1;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t res = 1;
            for (idx_t i = 1; i <= k; ++i) {
                res = res * ((seq_t)(n - k + i));
                res = res / (seq_t)i;
            }
            binom_tbl[n][k] = res;
        }
        for (idx_t k = n+1; k <= MAX_N; ++k)
            binom_tbl[n][k] = 0;
    }
}

/* Precompute all values needed for rows up to MAX_N. */
static void init_dp(void)
{
    init_binoms();

    dp[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        /* total number of digraphs on n vertices: 2^(n*(n-1)) */
        seq_t t = big_pow((seq_t)2, (seq_t)(n * (n - 1)));

        seq_t sum_rest = 0;

        /* compute dp[n][k] for k = n down to 2 */
        for (idx_t k = n; k >= 2; --k) {
            seq_t sum = 0;
            for (idx_t s = 1; s <= n - k + 1; ++s) {
                seq_t comb = binom_tbl[n-1][s-1];
                seq_t term = comb * f[s];
                term = term * dp[n-s][k-1];
                sum += term;
            }
            dp[n][k] = sum;
            sum_rest += sum;
        }

        /* f[n] = T(n,1) = total - sum_rest */
        f[n] = t - sum_rest;
        dp[n][1] = f[n];
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_dp();

    idx_t row, col;
    decompose(n, &row, &col);

    if (row > MAX_N || col > row) /* safety, should not happen for n<=10000 */
        return (seq_t)0;

    return dp[row][col];
}

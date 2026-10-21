/*
 * A206254  Number of (n+1)X8 0..3 arrays with every 2X2 subblock having zero
 *          permanent.
 * https://oeis.org/A206254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1)×8 0..3 arrays with every 2×2 subblock having
 *            zero permanent. Computed by dynamic programming over rows, where
 *            each row pattern gives a weight of 3^k (k = number of non‑zero
 *            entries). Compatibility of two consecutive rows only depends on
 *            the zero/non‑zero pattern, so 256 states (2^...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow3.h"

#define STATES (1 << 8)   /* number of 0/1 patterns for a row */

/* helper: test whether two 0/1 patterns are compatible */
static bool
compatible(unsigned a, unsigned b)
{
    for (int j = 0; j < 7; ++j) {
        if (((a >> j) & 1) && ((b >> (j + 1)) & 1))
            return false;
        if (((a >> (j + 1)) & 1) && ((b >> j) & 1))
            return false;
    }
    return true;
}

/* memoised data structure */
static bool       init_done = false;
static seq_t      weight[STATES];
static idx_t      compat_list[STATES][STATES];  /* indices of patterns compatible with given one */
static size_t    compat_count[STATES];

static void
precompute(void)
{
    /* precompute weights 3^k for all patterns */
    for (int i = 0; i < STATES; ++i) {
        int k = __builtin_popcount((unsigned)i);
        weight[i] = pow3((idx_t)k);
    }

    /* build compatibility lists */
    for (int i = 0; i < STATES; ++i) compat_count[i] = 0;
    for (int a = 0; a < STATES; ++a) {
        for (int b = 0; b < STATES; ++b) {
            if (compatible(a, b)) {
                compat_list[b][compat_count[b]++] = (idx_t)a;
            }
        }
    }
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;

    if (!init_done) {
        precompute();
        init_done = true;
    }

    idx_t height = n + 1;          /* number of rows */
    seq_t dp[STATES], ndp[STATES];
    seq_t zero = 0;
    for (int i = 0; i < STATES; ++i) {
        dp[i] = weight[i];         /* first row */
        ndp[i] = zero;
    }

    for (idx_t row = 1; row < height; ++row) {
        for (int t = 0; t < STATES; ++t) ndp[t] = zero;
        for (int t = 0; t < STATES; ++t) {
            seq_t sum = zero;
            size_t c = compat_count[t];
            for (size_t j = 0; j < c; ++j) {
                idx_t prev = compat_list[t][j];
                sum += dp[prev];
            }
            ndp[t] = sum * weight[t];
        }
        /* swap dp and ndp */
        for (int i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < STATES; ++i) ans += dp[i];
    return ans;
}

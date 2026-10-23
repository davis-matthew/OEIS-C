/*
 * A107266  Expansion of (1-6*x-sqrt((1-6*x)^2-4*6*x^2))/(2*6*x^2).
 * https://oeis.org/A107266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107266
 * Recurrence: (n+2)*a(n) = 6*(2*n+1)*a(n-1) - 12*(n-1)*a(n-2)
 * Initial values: a(0)=1, a(1)=6
 * This implementation uses 128‑bit arithmetic when available
 * to avoid overflow on built‑in integer types.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp = NULL;     /* cache of previously computed terms */
static idx_t dp_cap = 0;     /* allocated capacity of dp */
static idx_t dp_size = 0;    /* number of terms actually stored */

/* Ensure dp[n] exists, computing missing terms on demand. */
static void ensure_capacity(idx_t n)
{
    if (n < dp_size)
        return;

    /* Grow cache if necessary */
    if (dp_cap == 0) {
        dp_cap = 8;
        dp = (seq_t*)malloc(dp_cap * sizeof(seq_t));
    } else {
        while (dp_cap <= n) {
            dp_cap <<= 1;
            dp = (seq_t*)realloc(dp, dp_cap * sizeof(seq_t));
        }
    }

    /* Compute terms dp[dp_size .. n] */
    for (idx_t i = dp_size; i <= n; ++i) {
        if (i == 0) {
            dp[i] = (seq_t)1;
        } else if (i == 1) {
            dp[i] = (seq_t)6;
        } else {
#ifdef __SIZEOF_INT128__
            unsigned __int128 t1 = (unsigned __int128)dp[i-1] *
                                  (uint64_t)(6 * (2*i + 1));
            unsigned __int128 t2 = (unsigned __int128)dp[i-2] *
                                  (uint64_t)(12 * (i-1));
            unsigned __int128 num = t1 - t2;
            dp[i] = (seq_t)(num / (uint64_t)(i + 2));
#else
            seq_t t1 = dp[i-1] * (seq_t)(6 * (2*i + 1));
            seq_t t2 = dp[i-2] * (seq_t)(12 * (i-1));
            seq_t num = t1 - t2;
            dp[i] = num / (idx_t)(i + 2);
#endif
        }
    }
    dp_size = n + 1;
}

/* Public entry point – returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return dp[n];
}

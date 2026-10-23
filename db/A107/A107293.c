/*
 * A107293  The (1,1)-entry of the matrix M^n, where M is the 5 X 5 matrix
 *          [[0,1,0,0,0],[0,0,1,0,0], [0,0,0,1,0], [0,0,0,0,1], [1,0,-1,1,1]].
 * https://oeis.org/A107293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 1 a(n) = a(n-1) +
 *            a(n-2) - a(n-3) + a(n-5) for n ≥ 5 Recurrence implementation
 *            with memoization
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 65, u64 n <= 125,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
static seq_t *dp = NULL;   /* memoized values */
static size_t cap = 0;     /* allocated capacity */
static size_t sz  = 0;     /* number of initialized entries */

/* Ensure the memoization array can hold up to 'n' (inclusive). */
static void ensure_capacity(idx_t n) {
    size_t needed = (size_t)(n) + 1;
    if (cap >= needed) return;
    size_t newcap = cap ? cap << 1 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *newdp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
    if (!newdp) abort();  /* out of memory */
    dp = newdp;
    cap = newcap;
}

seq_t a(idx_t n) {
    /* Initialization of first 5 terms when array is empty. */
    if (sz == 0) {
        ensure_capacity(4);
        dp[0] = 0;
        dp[1] = 0;
        dp[2] = 0;
        dp[3] = 0;
        dp[4] = 1;
        sz = 5;
    }
    if (n < sz) return dp[(size_t)n];

    ensure_capacity(n);
    for (size_t i = sz; i <= (size_t)n; ++i) {
        if (i <= 4) {
            /* These values are already set, but for safety we handle here. */
            if (i < sz) continue;
            dp[i] = 0;
            if (i == 4) dp[i] = 1;
        } else {
            seq_t a1 = dp[i-1];
            seq_t a2 = dp[i-2];
            seq_t a3 = dp[i-3];
            seq_t a5 = dp[i-5];
            dp[i] = a1 + a2 - a3 + a5;
        }
    }
    sz = (size_t)n + 1;
    return dp[(size_t)n];
}

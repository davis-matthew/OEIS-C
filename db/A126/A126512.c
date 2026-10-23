/*
 * A126512  Number of base 17 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=17.
   For n>=2, let dp[n][d] be the number of length‑n base‑17 numbers ending
   with digit d (0≤d<17) such that adjacent digits differ by ≤4.
   Then dp[n][d] = Σ_{k=max(0,d-4)}^{min(16,d+4)} dp[n-1][k]
   and a(n)= Σ_{d=0}^{16} dp[n][d].
   The function memoises all computed terms up to the requested n. */

#include <stddef.h>
#include <stdbool.h>

/* upper bound for pre‑allocation: the tests use n≈10000. */
#define A126512_MAX_N 20000

seq_t a(idx_t n)
{
    static seq_t cache[A126512_MAX_N + 1];
    static seq_t dp_prev[17];
    static seq_t dp_next[17];
    static idx_t max_n  = 0;
    static bool  initialized = false;

    if (!initialized) {
        /* base case a(0) = 1 */
        cache[0] = (seq_t)1;

        /* one way for each digit for length 1 */
        for (int d = 0; d < 17; ++d)
            dp_prev[d] = (seq_t)1;
        seq_t total = 0;
        for (int d = 0; d < 17; ++d)
            total += dp_prev[d];
        cache[1] = total;          /* a(1) = 17 */

        max_n = 1;
        initialized = true;
    }

    if (n <= max_n)
        return cache[n];

    for (idx_t len = max_n + 1; len <= n; ++len) {
        for (int d = 0; d < 17; ++d) {
            int lo = d - 4;
            if (lo < 0) lo = 0;
            int hi = d + 4;
            if (hi > 16) hi = 16;
            seq_t s = 0;
            for (int k = lo; k <= hi; ++k)
                s += dp_prev[k];
            dp_next[d] = s;
        }
        seq_t total = 0;
        for (int d = 0; d < 17; ++d)
            total += dp_next[d];
        cache[len] = total;

        /* prepare for next length */
        for (int d = 0; d < 17; ++d)
            dp_prev[d] = dp_next[d];
    }
    max_n = n;
    return cache[n];
}

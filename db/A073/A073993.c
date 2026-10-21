/*
 * A073993  Number of strings of length n over Z_6 with trace 3 and subtrace
 *          4.
 * https://oeis.org/A073993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings of length n over Z_6 with trace 3 and
 *            subtrace 4. Recurrence: a(n; t, s) = Σ_{v=0..5} a(n-1; (t+v) mod
 *            6, (s+v*t) mod 6). Initial condition: a(0; 0,0)=1. The function
 *            returns a(n) for n ≥ 1; n < 1 yields 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;      /* flat array storing dp[row*36 + 6*t + s] */
static size_t dp_len = 0;     /* number of rows currently stored (0..dp_len-1) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    size_t target = (size_t)n;

    /* Initialise with the empty string DP[0] */
    if (dp_len == 0) {
        dp_len = 1;
        dp = (seq_t*)malloc(sizeof(seq_t) * 36);
        if (!dp) return (seq_t)0;              /* allocation failure */
        for (int i = 0; i < 36; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                     /* trace 0, subtrace 0 */
    }

    /* Extend the table up to row "target" if necessary */
    if (dp_len <= target) {
        size_t old_len = dp_len;
        dp_len = target + 1;
        dp = (seq_t*)realloc(dp, dp_len * sizeof(seq_t) * 36);
        if (!dp) return (seq_t)0;              /* allocation failure */

        /* Zero the new rows */
        for (size_t row = old_len; row < dp_len; ++row) {
            seq_t *rowptr = dp + row * 36;
            for (int j = 0; j < 36; ++j) rowptr[j] = (seq_t)0;
        }

        /* Compute the new rows sequentially */
        for (size_t i = old_len; i <= target; ++i) {
            seq_t *prev = dp + (i - 1) * 36;
            seq_t *curr = dp + i * 36;
            for (int t0 = 0; t0 < 6; ++t0) {
                for (int s0 = 0; s0 < 6; ++s0) {
                    seq_t ways = prev[t0 * 6 + s0];
                    if (ways == (seq_t)0) continue;
                    for (int v = 0; v < 6; ++v) {
                        int t1 = (t0 + v) % 6;
                        int s1 = (s0 + v * t0) % 6;
                        curr[t1 * 6 + s1] += ways;
                    }
                }
            }
        }
    }

    return dp[target * 36 + 3 * 6 + 4];   /* trace 3, subtrace 4 */
}

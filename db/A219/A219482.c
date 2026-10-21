/*
 * A219482  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 1, 9, 10, 19, 29, 48,
 *          ....
 * https://oeis.org/A219482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9734, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of representations of n as a sum of distinct elements of the
   generalized Fibonacci sequence starting 1, 9, 10, 19, 29, 48, ... */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static DP table: dp[s] = number of subsets of the computed
       Fibonacci numbers that sum to s. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    idx_t needed = n + 1u;

    /* Re‑allocate DP table if we are asked for a larger n. */
    if (dp == NULL || needed > dp_size) {
        seq_t *newdp = (seq_t *)malloc(needed * sizeof(seq_t));
        if (newdp == NULL) {                /* allocation failed */
            return (seq_t)0;
        }

        /* Initialise: only the empty subset sums to zero. */
        for (idx_t i = 0; i < needed; ++i) newdp[i] = (seq_t)0;
        newdp[0] = (seq_t)1;

        /* Generate the generalized Fibonacci numbers <= n. */
        idx_t fibs[64];
        idx_t fib_cnt = 0;

        /* The sequence starts 1, 9 */
        if (n >= (idx_t)1) fibs[fib_cnt++] = (idx_t)1;
        if (n >= (idx_t)9) fibs[fib_cnt++] = (idx_t)9;

        /* Generate subsequent terms as f[i] = f[i-1] + f[i-2] */
        idx_t f_prev = (idx_t)1;
        idx_t f_cur  = (idx_t)9;
        while (1) {
            idx_t next = f_prev + f_cur;
            if (next > n) break;
            if (fib_cnt < (idx_t)(sizeof(fibs) / sizeof(fibs[0]))) {
                fibs[fib_cnt++] = next;
            } else {
                /* Should not happen for n <= 10000, but keep safety. */
                break;
            }
            f_prev = f_cur;
            f_cur  = next;
        }

        /* DP over the selected Fibonacci numbers. */
        for (idx_t fi = 0; fi < fib_cnt; ++fi) {
            idx_t fv = fibs[fi];
            /* iterate sums from newdp_size-1 down to fv */
            for (idx_t s = needed - 1; s >= fv; --s) {
                newdp[s] += newdp[s - fv];
                if (s == fv) break;          /* avoid unsigned underflow */
            }
        }

        /* Replace the old table. */
        if (dp != NULL) free(dp);
        dp      = newdp;
        dp_size = needed;
    }

    return dp[n];
}

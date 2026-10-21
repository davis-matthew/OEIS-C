/*
 * A200255  Number of 0..n arrays x(0..5) of 6 elements with each no smaller
 *          than the sum of its previous elements modulo (n+1).
 * https://oeis.org/A200255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence by dynamic programming over prefix sums modulo (n+1).
 *            For n≥1, a(n) counts arrays x[0..5] with 0≤x[i]≤n such that for
 *            all i≥1: x[i] ≥ (x[0]+…+x[i‑1]) mod (n+1). The algorithm runs in
 *            O(n) time and uses DP over the modulo sums
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 70, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    const idx_t m = n + 1;           /* modulus */
    const size_t len = (size_t)m;   /* number of possible sums (0..m‑1) */

    /* DP arrays */
    seq_t *dp_cur   = (seq_t*)calloc(len,   sizeof(seq_t));
    seq_t *dp_next  = (seq_t*)calloc(len,   sizeof(seq_t));
    seq_t *start    = (seq_t*)calloc(len+1, sizeof(seq_t)); /* prefix increments */
    seq_t *end      = (seq_t*)calloc(len+1, sizeof(seq_t)); /* prefix decrements */

    if (!dp_cur || !dp_next || !start || !end) {
        free(dp_cur); free(dp_next); free(start); free(end);
        return 0;
    }

    /* after the first element, all sums 0..m-1 are reachable exactly once */
    for (size_t i = 0; i < len; ++i) dp_cur[i] = 1;

    /* process positions 1..5 (5 more elements) */
    for (int step = 1; step < 6; ++step) {
        /* reset auxiliary arrays */
        for (size_t i = 0; i <= len; ++i) {
            start[i] = 0;
            end[i]   = 0;
        }

        /* build range increments */
        for (idx_t s = 0; s < len; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            if (2*s < len) {                 /* S < (m/2) case */
                idx_t l1 = 2*s;                     /* interval [2S, m-1]  */
                start[l1] += val;
                end[len]   += val;                   /* sentinel to cancel after m-1 */

                if (s > 0) {                        /* optional interval [0, S-1] */
                    start[0] += val;
                    end[s]   += val;                 /* S-1+1 = S */
                }
            } else {                             /* S >= (m/2) case */
                idx_t l = 2*s - len;                /* interval [2S-m, S-1] */
                idx_t r = s - 1;
                start[l] += val;
                end[r+1] += val;                    /* r+1 = S */
            }
        }

        /* compute dp_next via prefix sums */
        seq_t cum_start = 0;
        seq_t cum_end   = 0;
        for (idx_t i = 0; i < len; ++i) {
            cum_start += start[i];
            cum_end   += end[i];
            dp_next[i] = cum_start - cum_end;     /* safe: cum_start ≥ cum_end */
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp  = dp_cur;
        dp_cur      = dp_next;
        dp_next     = tmp;
    }

    /* sum all final states to get the answer */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) result += dp_cur[i];

    /* clean up */
    free(dp_cur);
    free(dp_next);
    free(start);
    free(end);

    return result;
}

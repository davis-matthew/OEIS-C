/*
 * A198324  Number of lattice paths from (0,0) to (n,0) that do not go below
 *          the x-axis or above the diagonal x=y and consist of steps U=(1,1),
 *          D=(1,-1) and S=(0,1).
 * https://oeis.org/A198324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 34, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198324:
 *
 * Number of lattice paths from (0,0) to (n,0) that stay inside the
 * triangle bounded by y = 0 and y = x, using steps
 * U = (1,1), D = (1,-1) and S = (0,1).
 *
 * The algorithm uses a dynamic programming on the horizontal step
 * counter h and the current deficit d = x – y.  The deficit always
 * satisfies 0 ≤ d ≤ h.  Transitions:
 *
 *   * U : (h,d) → (h+1,d)          (deficit unchanged)
 *   * D : (h,d) → (h+1,d+2)        allowed only if d ≤ h–1
 *   * S : (h,d) → (h,d–1)          allowed only if d ≥ 1
 *
 * Between horizontal steps any number of S‑steps may be taken.
 * After each horizontal step we apply all possible S‑steps
 * in that layer (cumulative sum in descending order).  This
 * gives the exact count in O(n²) time and O(n) memory.
 *
 * The function is valid for every integer type defined for seq_t
 * and idx_t. */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n)
{
    /* Allocate tables */
    seq_t *dp   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *tmp  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (dp == NULL || tmp == NULL) {
        /* allocation failed – abort (not expected in tests) */
        return (seq_t)0;
    }

    dp[0] = 1;                 /* start at h = 0, deficit d = 0 */

    for (idx_t h = 0; h < n; ++h) {
        /* Apply any number of S-steps at current horizontal level */
        if (h > 0) {
            for (idx_t d = h - 1; ; --d) {
                dp[d] += dp[d + 1];
                if (d == 0) break;
            }
        }

        /* Reset tmp up to h+1 (new deficit range after one more U/D) */
        for (idx_t i = 0; i <= h + 1; ++i) tmp[i] = 0;

        /* Transitions using U and D */
        for (idx_t d = 0; d <= h; ++d) {
            seq_t val = dp[d];
            if (val == 0) continue;

            /* U step, deficit unchanged */
            tmp[d] += val;

            /* D step, deficit increases by 2 (allowed only if d <= h-1) */
            if (d <= h - 1) {
                tmp[d + 2] += val;
            }
        }

        /* Copy tmp back into dp for the next iteration */
        for (idx_t i = 0; i <= h + 1; ++i) dp[i] = tmp[i];
    }

    /* After the last horizontal step, apply all possible final S-steps */
    if (n > 0) {
        for (idx_t d = n - 1; ; --d) {
            dp[d] += dp[d + 1];
            if (d == 0) break;
        }
    }

    seq_t result = dp[n];

    /* Free allocated memory */
    free(dp);
    free(tmp);

    return result;
}

/*
 * A096611  a(n)=number of Catalan knight paths in right half-plane from (0,0)
 *          to (n,2).
 * https://oeis.org/A096611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Catalan knight paths in the right half‑plane
 *            from (0,0) to (n,2). A linear recurrence on the y‑coordinate is
 *            used, with three rotating arrays of size 4*n+1. See the OEIS
 *            definition
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The reference value for n==0 is 0. */
    if (n == 0) return 0;

    /* Each x has possible y values from –2x to +2x.  Across all
       x ≤ n the range is –2n … +2n, hence size = 4*n+1. */
    const size_t total = 4 * (size_t)n + 1;
    const size_t base   = 2 * (size_t)n;          /* y=0 maps to this index */

    /* Allocate the three arrays for x-2, x-1 and the current x. */
    seq_t *a0 = (seq_t*)calloc(total, sizeof(seq_t)); /* x‑2  */
    seq_t *a1 = (seq_t*)calloc(total, sizeof(seq_t)); /* x‑1  */
    seq_t *a2 = (seq_t*)calloc(total, sizeof(seq_t)); /* x    */

    if (!a0 || !a1 || !a2) {
        /* On real builds we would handle OOM.  For the exercise return 0. */
        free(a0); free(a1); free(a2);
        return 0;
    }

    /* Initial condition: one path from (0,0) to (0,0).  This is in a1
       because a1 holds counts for x‑1 = 0 at the start of the loop. */
    a1[base] = 1;

    seq_t *prev2 = a0;          /* array for x-2  */
    seq_t *prev1 = a1;          /* array for x-1  */
    seq_t *cur   = a2;          /* array for current x */

    for (idx_t x = 1; x <= n; ++x) {
        /* Current y range for this x is –2x … +2x. */
        size_t minY = base - 2 * (size_t)x;
        size_t maxY = base + 2 * (size_t)x;
        for (size_t i = minY; i <= maxY; ++i) cur[i] = 0;

        /* Contributions from x-1 regardless of x>=1. */
        size_t minY_prev1 = base - 2 * (size_t)(x - 1);
        size_t maxY_prev1 = base + 2 * (size_t)(x - 1);
        for (size_t i = minY_prev1; i <= maxY_prev1; ++i) {
            seq_t v = prev1[i];
            if (v) {
                cur[i - 2] += v;   /* move (1,2)  */
                cur[i + 2] += v;   /* move (1,-2) */
            }
        }

        /* Contributions from x-2 when x≥2. */
        if (x >= 2) {
            size_t minY_prev2 = base - 2 * (size_t)(x - 2);
            size_t maxY_prev2 = base + 2 * (size_t)(x - 2);
            for (size_t i = minY_prev2; i <= maxY_prev2; ++i) {
                seq_t v = prev2[i];
                if (v) {
                    cur[i - 1] += v;   /* move (2,1)  */
                    cur[i + 1] += v;   /* move (2,-1) */
                }
            }
        }

        /* Rotate arrays for next iteration:
           prev2 <- prev1, prev1 <- cur, cur <- prev2. */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    /* After the last iteration prev1 holds the counts for x=n. */
    seq_t result = prev1[base + 2];  /* y = 2 */

    free(a0);
    free(a1);
    free(a2);

    return result;
}

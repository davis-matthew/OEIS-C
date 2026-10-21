/*
 * A227614  Number of partitions of n into distinct parts with perimeter n-2.
 * https://oeis.org/A227614
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A227344(n,n-2) (partitions of n into distinct parts with
 *            perimeter n‑2)
 * Range:     u8 n <= 53, u16 n <= 115, u32 n <= 302, u64 n <= 927,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;                     /* undefined below the offset */

    /* the part [1,2,3] is mandatory; the remaining sum to distribute starts from 5 */
    idx_t target = n - 6;              /* sum of the remaining distinct parts */
    if (target == 0)
        return 1;                      /* only the partition [1,2,3] */

    const size_t sz = (size_t)target + 1;   /* array size for each run-length state */

    /* current DP arrays: dp0,dp1,dp2 correspond to run lengths 0,1,2 ending at i‑1 */
    seq_t *dp0 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp0 || !dp1 || !dp2) return 0;    /* allocation failure */

    dp0[0] = 1;                                   /* empty subset */

    /* temporary arrays for next state */
    seq_t *ndp0 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *ndp1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *ndp2 = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!ndp0 || !ndp1 || !ndp2) {
        free(dp0); free(dp1); free(dp2);
        return 0;
    }

    /* number 4 is forbidden (otherwise part 3 would have two neighbors) */
    for (idx_t val = 5; val <= target; ++val) {
        /* reset next arrays */
        memset(ndp0, 0, sz * sizeof(seq_t));
        memset(ndp1, 0, sz * sizeof(seq_t));
        memset(ndp2, 0, sz * sizeof(seq_t));

        size_t v = (size_t)val;
        for (size_t s = 0; s <= target; ++s) {
            seq_t a0 = dp0[s];
            seq_t a1 = dp1[s];
            seq_t a2 = dp2[s];

            /* skip current value: run length resets to 0 */
            if (a0 || a1 || a2)
                ndp0[s] += a0 + a1 + a2;

            /* pick current value if allowed */
            if (s + v <= (size_t)target) {
                if (a0) ndp1[s + v] += a0;   /* run length becomes 1 */
                if (a1) ndp2[s + v] += a1;   /* run length becomes 2 */
                /* cannot pick when a2 because that would create a run of 3 */
            }
        }

        /* swap current and next pointers */
        seq_t *tmp = dp0; dp0 = ndp0; ndp0 = tmp;
        tmp = dp1; dp1 = ndp1; ndp1 = tmp;
        tmp = dp2; dp2 = ndp2; ndp2 = tmp;
    }

    seq_t result = dp0[target] + dp1[target] + dp2[target];

    free(dp0); free(dp1); free(dp2);
    free(ndp0); free(ndp1); free(ndp2);
    return result;
}

/*
 * A068788  S(n; 1,3) = S(n; 3,3) where S(n; t,s) is the number of length n
 *          4-ary strings whose digits sum to t mod 4 and whose sum of
 *          products of all pairs of digits sum to s mod 4.
 * https://oeis.org/A068788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = S(n; 1,3) = S(n; 3,3), where S(n; t,s) counts the 4-ary
 *            strings of length n with digit-sum ≡ t (mod 4) and
 *            pair-product-sum ≡ s (mod 4). Recurrence derived from adding one
 *            digit: S(n; t,s) = Σ_{d=0}^3 S(n-1; t-d, s-d*(t-d)) (mod 4)
 *            which yields an O(n) DP over the 16 (t,s) states
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle n < 1 defensively (offset is 1) */
    if (n < 1) return 0;

    /* static state: computed values up to current_max. */
    static seq_t *vals = NULL;          /* a values, 1-indexed */
    static idx_t current_max = 0;       /* highest n computed */
    static idx_t alloc_cap = 0;         /* capacity of vals array */

    /* DP arrays for current and previous step (16 states each). */
    static seq_t prev[16] = {0};
    static seq_t curr[16] = {0};
    static int initialized = 0;         /* flag to set prev[0][0] = 1 */

    /* initialise DP base for n=0 if first time */
    if (!initialized) {
        for (int i = 0; i < 16; ++i) prev[i] = 0;
        prev[0] = 1;                     /* state t=0, s=0 */
        initialized = 1;
    }

    /* extend vals array if needed */
    if (vals == NULL) {
        alloc_cap = (n >= 16 ? n : 16);
        vals = (seq_t *)malloc(alloc_cap * sizeof(seq_t));
    }
    while (alloc_cap <= n) {
        idx_t new_cap = alloc_cap * 2;
        if (new_cap <= alloc_cap) new_cap = alloc_cap + 1024; /* avoid overflow */
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < current_max; ++i) tmp[i] = vals[i];
        free(vals);
        vals = tmp;
        alloc_cap = new_cap;
    }

    /* compute DP up to requested n if not yet done */
    for (idx_t step = current_max + 1; step <= n; ++step) {
        /* clear curr */
        for (int i = 0; i < 16; ++i) curr[i] = 0;

        /* transition from prev to curr */
        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t val = prev[t * 4 + s];
                if (val == (seq_t)0) continue;
                for (int d = 0; d < 4; ++d) {
                    int t1 = (t + d) & 3;
                    int s1 = (s + d * t) & 3;
                    curr[t1 * 4 + s1] += val;
                }
            }
        }

        /* store answer for this step */
        vals[step] = curr[1 * 4 + 3];    /* S(step; 1,3) */

        /* prepare for next iteration */
        for (int i = 0; i < 16; ++i) prev[i] = curr[i];
    }

    current_max = n;
    return vals[n];
}

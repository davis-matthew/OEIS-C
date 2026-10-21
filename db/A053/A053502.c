/*
 * A053502  Number of degree-n permutations of order dividing 12.
 * https://oeis.org/A053502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d ∈ {1,2,3,4,6,12} , d ≤ n} (n-1)!/(n-d)! * a(n-d),
 *            a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence from the cycle‑index formula for
   permutations whose cycle lengths divide 12. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;   /* array of a values */
    static idx_t max_cached = 0;  /* highest index currently cached (a[max_cached]) */

    /* on first call, create cache[0] = 1 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        max_cached = 0;
    }

    /* compute forward until we have a(n) */
    while (max_cached < n) {
        idx_t cur = max_cached + 1;    /* new index to compute */
        seq_t sum = (seq_t)0;

        /* allowed cycle lengths whose lcm divides 12 */
        const idx_t cycle_len[] = {1, 2, 3, 4, 6, 12};
        const size_t cycle_cnt = sizeof(cycle_len) / sizeof(cycle_len[0]);

        for (size_t i = 0; i < cycle_cnt; ++i) {
            idx_t d = cycle_len[i];
            if (d > cur) continue;      /* cycle too long for this n */

            /* multiplier = (cur-1)! / (cur-d)! =
               product (cur-1) * (cur-2) * ... * (cur-(d-1))  */
            seq_t multiplier = (seq_t)1;
            for (idx_t k = 0; k < d - 1; ++k) {
                multiplier *= (seq_t)(cur - 1 - k);
            }

            sum += multiplier * cache[cur - d];
        }

        /* extend cache to hold a[cur] */
        cache = (seq_t*)realloc(cache, (cur + 1) * sizeof(seq_t));
        cache[cur] = sum;
        max_cached = cur;
    }

    return cache[(size_t)n];
}

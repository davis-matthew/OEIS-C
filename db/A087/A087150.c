/*
 * A087150  Monotonically increasing sequence of least positive integers,
 *          a(1)=1, such that the self-convolution produces all squares.
 * https://oeis.org/A087150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Recurrence:
   a(1) = 1;
   For n > 1, a(n) is the smallest integer > a(n-1) such that
   2*a(n) + Σ_{k=2}^{n-1} a(k)*a(n-k+1) is a perfect square. */
seq_t a(idx_t n) {
    /* static memoisation cache (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_size = 0;   /* highest n that has been computed */

    if (n == 0)
        return 0;   /* undefined for n <= 0 */

    /* ensure cache large enough to hold indices up to n */
    size_t need = (size_t)n + 1;      /* we store cache[1]..cache[n] */
    if (need > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity : 1;
        while (new_cap < need)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i) {
        size_t idx = (size_t)i;
        if (idx == 1) {            /* a(1) = 1 */
            cache[1] = (seq_t)1;
            continue;
        }

        /* Compute partial sum t = Σ_{k=2}^{i-1} a(k)*a(i-k+1) */
        seq_t t = 0;
        for (idx_t k = 2; k <= i - 1; ++k) {
            size_t ki = (size_t)k;
            size_t ji = (size_t)(i - k + 1);
            t += cache[ki] * cache[ji];
        }

        seq_t prev = cache[idx - 1];

        seq_t s = floor_sqrt(t);
        if (s * s < t)          /* make s the ceiling of sqrt(t) */
            ++s;

        seq_t ans = 0;
        while (true) {
            seq_t diff = s * s - t;
            if (diff > (seq_t)0 && ((diff & (seq_t)1) == (seq_t)0)) {
                seq_t candidate = diff / 2;
                if (candidate > prev) {
                    ans = candidate;
                    break;
                }
            }
            ++s;
        }

        cache[idx] = ans;
    }

    cache_size = (size_t)n;
    return cache[n];
}

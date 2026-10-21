/*
 * A204843  Expansion of x * f(-x^24) + (3 * phi(-x^36) - phi(-x^4)) / 2 in
 *          powers of x where phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A204843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * φ(−x^36) + x·f(−x^24) +
 *            x^4·f(−x^12,−x^60), * which equals * ∑_{k∈ℤ} (−1)^{k(k+1)/2}
 *            x^{9k²} * + ∑_{k∈ℤ} (−1)^{k(k−1)/2} x^{1+9k²−6k}
 * Range:     i8 n <= 9942, i16 n <= 9942, i32 n <= 9942, i64 n <= 9942,
 *            i128 n <= 9942, big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_cache.h"

static seq_t *seq = NULL;
static idx_t seq_cap = 0;

seq_t a(idx_t n)
{
    /* Resize and recompute the cache if we need higher indices. */
    if (n >= seq_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq = alloc_cache((idx_t)new_cap);
        for (idx_t i = 0; i < new_cap; ++i)
            seq[i] = (seq_t)0;

        /* Bound for k in the first sum 9k² ≤ n. */
        int64_t bound1 = 0;
        while (9 * (bound1 + 1) * (bound1 + 1) <= (int64_t)n)
            ++bound1;

        /* Bound for k in the second sum 1+9k²-6k ≤ n. */
        int64_t bound2 = 0;
        while (1 + 9 * (bound2 + 1) * (bound2 + 1) - 6 * (bound2 + 1) <= (int64_t)n)
            ++bound2;

        int64_t bound = bound1 > bound2 ? bound1 : bound2;
        ++bound;  /* safety margin */

        for (int64_t k = -bound; k <= bound; ++k)
        {
            /* First sum: exponent 9k². */
            int64_t e1 = 9 * k * k;
            if (e1 <= (int64_t)n)
            {
                int64_t t1 = k * (k + 1) / 2;
                seq_t s1 = (t1 & 1LL) ? (seq_t)-1 : (seq_t)1;
                seq[(idx_t)e1] += s1;
            }

            /* Second sum: exponent 1+9k²-6k. */
            int64_t e2 = 1 + 9 * k * k - 6 * k;
            if (e2 >= 0 && e2 <= (int64_t)n)
            {
                int64_t t2 = k * (k - 1) / 2;
                seq_t s2 = (t2 & 1LL) ? (seq_t)-1 : (seq_t)1;
                seq[(idx_t)e2] += s2;
            }
        }

        seq_cap = new_cap;
    }
    return seq[n];
}

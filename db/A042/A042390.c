/*
 * A042390  Numerators of continued fraction convergents to sqrt(722).
 * https://oeis.org/A042390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(722) Recurrence:
 *            p_-1 = 1, p_0 = floor(sqrt(722)) = 26, p_n = a_n * p_{n-1} +
 *            p_{n-2} for n ≥ 1 where a_n for n ≥ 1 is periodic with period
 *            14: [1, 6, 1, 2, 3, 2, 26, 2, 3, 2, 1, 6, 1, 52] The sequence
 *            a(n) is p_n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Periodic partial quotients of sqrt(722) after a0 = 26. */
static const seq_t period[14] = {
    (seq_t)1, (seq_t)6, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)2,
    (seq_t)26, (seq_t)2, (seq_t)3, (seq_t)2, (seq_t)1, (seq_t)6,
    (seq_t)1, (seq_t)52
};
static const idx_t period_len = 14;
static const seq_t a0_val = (seq_t)26;

/* Cached values of a(k) for 0 ≤ k ≤ last_cached. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_cached = (idx_t)-1;   /* -1 means no value cached yet */

seq_t a(idx_t n)
{
    /* Ensure cache[0] is initialized when n == 0. */
    if (n == 0) {
        if (last_cached == (idx_t)-1) {
            if (cache_cap < 1) {
                cache_cap = 1;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            cache[0] = a0_val;
            last_cached = 0;
        }
        return cache[0];
    }

    /* Return cached value if already computed. */
    if (n <= last_cached) {
        return cache[n];
    }

    /* Expand cache if necessary. */
    idx_t required = n + 1;
    if ((size_t)required > cache_cap) {
        size_t new_cap = (size_t)required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 (better than crash). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t idx = last_cached + 1; idx <= n; ++idx) {
        seq_t a_k = (idx == 0) ? a0_val : period[(idx - 1) % period_len];
        seq_t prev = cache[idx - 1];
        seq_t prev2 = (idx >= 2) ? cache[idx - 2] : (seq_t)1;
        cache[idx] = a_k * prev + prev2;
    }

    last_cached = n;
    return cache[n];
}

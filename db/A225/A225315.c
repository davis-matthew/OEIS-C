/*
 * A225315  Number of 8Xn -1,1 arrays such that the sum over i=1..8,j=1..n of
 *          i*x(i,j) is zero and rows are nondecreasing (ways to put n
 *          thrusters pointing east or west at each of 8 positions 1..n
 *          distance from the hinge of a south-pointing gate without turning
 *          the gate).
 * https://oeis.org/A225315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 8×n {−1,1} arrays with nondecreasing rows and
 *            total weighted sum ∑_{i=1..8,j=1..n} i·x(i,j)=0. Empirical
 *            recurrence (order 36): a(n) = a(n-1)+a(n-2)-a(n-5)-a(n-7)-a(n-9)
 *            +a(n-11)+2a(n-12)+a(n-13)+a(n-15)
 *            -a(n-16)-a(n-17)-2a(n-18)-a(n-19)-a(n-20)
 *            +a(n-21)+a(n-23)+2a(n-24)+a(n-25)...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 32, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   compute_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_base.h"

static const size_t shifts[] = {
    1, 2, 5, 7, 9, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25,
    27, 29, 31, 34, 35, 36
};
static const int   coeffs[] = {
     1,  1, -1, -1, -1,  1,  2,  1,  1, -1, -1, -2, -1, -1,  1,  1,  2,  1,
    -1, -1, -1,  1,  1, -1
};
static const size_t RECUR_SOURCES = sizeof(shifts) / sizeof(shifts[0]);

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache to store computed values. */
    static seq_t *cache = NULL;      /* 1‑based index: cache[0] = a(1) */
    static size_t capacity = 0;      /* allocated size */
    static size_t cached_n   = 0;    /* highest n computed so far */

    /* Initialise cache if first call. */
    if (cache == NULL) {
        capacity = 36;                 /* at least up to 36 for recurrence */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        compute_base(cache);           /* fill a(1) … a(36) */
        cached_n = 36;
    }

    /* Grow cache if necessary. */
    if (n > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* If value already computed, return it. */
    if (n <= cached_n)
        return cache[n - 1];

    /* Compute new terms using the recurrence. */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (size_t s = 0; s < RECUR_SOURCES; ++s) {
            size_t shift = shifts[s];
            int coeff = coeffs[s];
            idx_t idx = k - 1 - shift;   /* a(k - shift) */
            seq_t v = cache[idx];
            if (coeff > 0)
                pos += v * (seq_t)coeff;
            else
                neg += v * (seq_t)(-coeff);
        }
        cache[k - 1] = pos - neg;
    }
    cached_n = n;
    return cache[n - 1];
}

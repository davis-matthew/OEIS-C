/*
 * A231185  Coefficients of the nonnegative powers of rho(11) = 2*cos(Pi/11)
 *          when written in the power basis of the degree 5 number field
 *          Q(rho(11)). Coefficients of the third power.
 * https://oeis.org/A231185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5) for n ≥
 *            0 with a(-5) = -2, a(-4) = -1, a(-3) = a(-2) = a(-1) = 0 The
 *            first five terms are a(0)=1, a(1)=0, a(2)=4, a(3)=1, a(4)=14
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values of a(n)   */
static idx_t  cache_cap = 0;     /* allocated capacity        */
static idx_t  cache_size = 0;    /* number of entries filled  */

/* Ensure that cache[n] is computed and stored. */
static void ensure_cache(idx_t n) {
    /* Initialise cache if it does not yet exist. */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 5;
        /* Base values a(0)..a(4) */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 4;
        cache[3] = 1;
        cache[4] = 14;
    }

    /* If n is already covered, nothing to do. */
    if (n < cache_size) return;

    /* Grow the cache array if necessary. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute values up to n using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i - 1]
                  + (seq_t)4 * cache[i - 2]
                  - (seq_t)3 * cache[i - 3]
                  - (seq_t)3 * cache[i - 4]
                  + cache[i - 5];
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 0. */
    if (n < 0) return 0;   /* Off‑by‑default for safety. */
    ensure_cache(n);
    return cache[n];
}

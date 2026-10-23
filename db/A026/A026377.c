/*
 * A026377  a(n) = number of integer strings s(0),...,s(n) counted by array T
 *          in A026374 that have s(n)=4; also a(n) = T(2n,n-2).
 * https://oeis.org/A026377
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^(n+2)](1+3t+t^2)^n. Computed by iteratively
 *            multiplying the polynomial (1+3t+t^2)^k and keeping only the
 *            coefficient of t^(k+2). The implementation stores the polynomial
 *            coefficients in an array and updates them in place, using the
 *            recurrence new[t] = old[t] + 3*old[t-1] + old[t-2] (for...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/free_seq_arr.h"

/* Helper allocation / deallocation that works for both C and C++ */
static inline seq_t *alloc_seq(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Sequence cache and polynomial state */
static seq_t *poly = NULL;        /* polynomial coefficients   */
static size_t poly_cap = 0;       /* allocated capacity        */
static idx_t poly_deg = 0;        /* current degree (2*n)      */

static seq_t *a_cache = NULL;     /* cached a(n) values        */
static size_t cache_cap = 0;      /* allocated capacity        */
static idx_t max_n_computed = 0;  /* highest n already computed */

static int initialized = 0;       /* one‑time initialization flag */

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* initialise static data on first call */
    if (!initialized) {
        /* start with P_0(t) = 1 */
        poly_cap = 1;
        poly = alloc_seq(poly_cap);
        poly[0] = (seq_t)1;
        poly_deg = 0;

        /* cache a(0) and a(1) = 0 (unused) */
        cache_cap   = 2;
        a_cache     = alloc_seq(cache_cap);
        a_cache[0]  = (seq_t)0;
        a_cache[1]  = (seq_t)0;

        max_n_computed = 0;
        initialized = 1;
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = a_cache[i];
        free_seq_arr(a_cache);
        a_cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t step = max_n_computed + 1; step <= n; ++step) {
        /* new polynomial degree: old_deg + 2 */
        idx_t new_deg = poly_deg + 2;
        size_t needed_cap = (size_t)new_deg + 1;
        if (!poly || poly_cap < needed_cap) {
            /* allocate bigger array and copy old data */
            seq_t *new_poly = alloc_seq(needed_cap);
            for (size_t i = 0; i <= (size_t)poly_deg; ++i)
                new_poly[i] = poly[i];
            /* zero the rest (not strictly necessary, but clean) */
            for (size_t i = (size_t)poly_deg + 1; i < needed_cap; ++i)
                new_poly[i] = (seq_t)0;
            free_seq_arr(poly);
            poly = new_poly;
            poly_cap = needed_cap;
        }

        /* in‑place update from high to low */
        for (int t = (int)new_deg; t >= 0; --t) {
            seq_t val = (t <= (int)poly_deg) ? poly[t] : (seq_t)0;
            if (t >= 1) val += poly[t - 1] * (seq_t)3;
            if (t >= 2) val += poly[t - 2];
            poly[t] = val;
        }

        poly_deg = new_deg;

        /* store a(step) = coefficient of t^(step+2) */
        a_cache[step] = (step + 2 <= new_deg) ? poly[step + 2] : (seq_t)0;
    }

    max_n_computed = n;
    return a_cache[n];
}

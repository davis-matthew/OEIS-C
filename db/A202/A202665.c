/*
 * A202665  Number of -n..n arrays of 5 elements with first through fourth
 *          differences also in -n..n.
 * https://oeis.org/A202665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3)+2*a(n-4)+a(n-5)-4*a(n-7)-3*a(n-8)-4*a(n-9) *
 *            +2*a(n-11)+5*a(n-12)+5*a(n-13)+2*a(n-14)-4*a(n-16)-3*a(n-17) *
 *            -4*a(n-18)+a(n-20)+2*a(n-21)+2*a(n-22)-a(n-25) * on indices n ≥
 *            26 with a(1)…a(25) given by direct enumeration
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 73, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   exact_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/exact_compute.h"

/* static cache of computed values, 1‑based */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_n = 0;

/* ------------------------------------------------------------------ */
/* Compute a(n) for n <= 25 by explicit enumeration of all arrays      */

/* ------------------------------------------------------------------ */
/* Compute a(n) for n ≥ 26 using the linear recurrence.               */
static seq_t recurrence_compute(idx_t m)
{
    /* All operands are non‑negative, so unsigned arithmetic is safe. */
    seq_t res = 0;
    res += cache[m - 3] * 2;
    res += cache[m - 4] * 2;
    res += cache[m - 5];
    res -= cache[m - 7] * 4;
    res -= cache[m - 8] * 3;
    res -= cache[m - 9] * 4;
    res += cache[m - 11] * 2;
    res += cache[m - 12] * 5;
    res += cache[m - 13] * 5;
    res += cache[m - 14] * 2;
    res -= cache[m - 16] * 4;
    res -= cache[m - 17] * 3;
    res -= cache[m - 18] * 4;
    res += cache[m - 20];
    res += cache[m - 21] * 2;
    res += cache[m - 22] * 2;
    res -= cache[m - 25];
    return res;
}

/* ------------------------------------------------------------------ */
/* Ensure that cache has values up to `needed`.                        */
static void grow_cache(idx_t needed)
{
    if (needed <= cached_n)
        return;

    size_t new_cap = (size_t)(needed + 1);   /* indices 0..needed */
    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
        /* zero-initialize new space */
        for (size_t i = cached_n + 1; i < new_cap; ++i)
            cache[i] = 0;
    }

    for (idx_t m = cached_n + 1; m <= needed; ++m) {
        if (m <= 25)
            cache[m] = exact_compute(m);
        else
            cache[m] = recurrence_compute(m);
    }
    cached_n = needed;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* sequence is defined for n ≥ 1 */
    grow_cache(n);
    return cache[n];
}

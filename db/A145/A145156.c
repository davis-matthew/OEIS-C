/*
 * A145156  Number of Greek-key tours on a 5 X n board; i.e., self-avoiding
 *          walks on 5 X n grid starting in top left corner.
 * https://oeis.org/A145156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence of order 13: * * a(1) = 1,
 *            a(2) = 5, a(3) = 38, a(4) = 160, a(5) = 824, * a(6) = 3501, a(7)
 *            = 16262, a(8) = 68591, a(9) = 304177, * a(10) = 1276805, a(11) =
 *            5522791, a(12) = 23117164, * a(13) = 98562435, a(14) =
 *            411870513. * * For n ≥ 15 * * a(n) = 4 a(n-1) +...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Recurrence coefficients, indexed by k-1 where k is the offset
   from n.  For a(n) the term a(n-k) uses Dcoeff[k-1].
   Dcoeff[0] corresponds to a(n-1), Dcoeff[1] to a(n-2), etc. */
static const int Dcoeff[13] = {
    4,   14,   -54,  -33,  117,  2,   -84,  -6,   9,
    0,   -14,  0,   -2
};

/* The first 14 terms, index 0 unused. */
static const seq_t base_terms[15] = {
    (seq_t)0,
    (seq_t)1,    (seq_t)5,      (seq_t)38,
    (seq_t)160,  (seq_t)824,    (seq_t)3501,
    (seq_t)16262, (seq_t)68591, (seq_t)304177,
    (seq_t)1276805, (seq_t)5522791, (seq_t)23117164,
    (seq_t)98562435, (seq_t)411870513
};

/* Cached values.  cache[0] is unused (set to 0 for safety). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of cache array */
static idx_t cached_max = 0;   /* maximum n that has been computed */

/* Grow the cache array to accommodate at least 'need' indices (inclusive). */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    if (!new_cache)
        abort();
    if (cache) {
        /* Copy already stored values. */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
    cache = new_cache;
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* Compute terms up to and including n, storing them in the cache. */
static void compute_up_to(idx_t n)
{
    if (!cache) {
        /* Initialize cache with base terms. */
        ensure_cache(15 + 1);
        cache[0] = (seq_t)0;
        for (size_t i = 1; i <= 14; ++i)
            cache[i] = base_terms[i];
        cached_max = 14;
    }

    if (n <= cached_max)
        return;

    ensure_cache((size_t)n + 1);

    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;

        for (int i = 0; i < 13; ++i) {
            int dk = Dcoeff[i];
            if (!dk)
                continue;

            idx_t idx = k - (i + 1);      /* a(k-(i+1)) */
            seq_t term = cache[idx] * (seq_t)(dk > 0 ? dk : -dk);

            if (dk > 0)
                pos += term;
            else
                neg += term;
        }
        cache[k] = pos - neg;  /* guaranteed non‑negative */
    }

    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined, but harmless */

    if (n <= 14)
        return base_terms[n];

    compute_up_to(n);
    return cache[n];
}

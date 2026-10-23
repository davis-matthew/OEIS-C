/*
 * A198639  Number of 2n X 4 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to an even number of horizontal
 *          and vertical neighbors.
 * https://oeis.org/A198639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 78*a(n-1) - 569*a(n-2) - 11744*a(n-3) + 71840*a(n-4) +
 *            745984*a(n-5) - 2507008*a(n-6) - 18538496*a(n-7) +
 *            36110336*a(n-8) + 142606336*a(n-9) - 268435456*a(n-10) Initial
 *            terms: a(1)=36,a(2)=2436,a(3)=162776,a(4)=10937316,
 *            a(5)=734979136,a(6)=49393306556,a(7)=3319433387656,
 *            a(8)=223079838087...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms */
static seq_t *cache   = NULL; /* array of a(i) for i >= 0 */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_size = 0; /* highest index computed (>=1) */

/* Known initial terms (index 0 unused) */
static const seq_t init[11] = {
    0ULL,                         /* a(0) unused */
    36ULL,                        /* a(1) */
    2436ULL,                      /* a(2) */
    162776ULL,                    /* a(3) */
    10937316ULL,                  /* a(4) */
    734979136ULL,                 /* a(5) */
    49393306556ULL,               /* a(6) */
    3319433387656ULL,             /* a(7) */
    223079838087956ULL,           /* a(8) */
    14991902921730896ULL,         /* a(9) */
    1007518927448282476ULL        /* a(10) */
};

/* Ensure that cache holds terms up to index n (inclusive) */
static void ensure_cache(size_t n)
{
    if (cache_size >= n)
        return;

    /* Grow the cache if needed */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < n + 1)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i];
        } else {
            seq_t pos = 0, neg = 0;
            /* positive contributions */
            pos += ((seq_t)78)      * cache[i-1];
            pos += ((seq_t)71840)   * cache[i-4];
            pos += ((seq_t)745984)  * cache[i-5];
            pos += ((seq_t)36110336) * cache[i-8];
            pos += ((seq_t)142606336) * cache[i-9];
            /* negative contributions */
            neg += ((seq_t)569)     * cache[i-2];
            neg += ((seq_t)11744)   * cache[i-3];
            neg += ((seq_t)2507008) * cache[i-6];
            neg += ((seq_t)18538496)* cache[i-7];
            neg += ((seq_t)268435456)*cache[i-10];
            cache[i] = pos - neg;   /* safe: pos >= neg for all i */
        }
    }
    cache_size = n;
}

/* Public function required by the spec */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* indices below 1 are undefined */
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}

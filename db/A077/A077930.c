/*
 * A077930  Expansion of (1-x)^(-1)/(1+2*x+x^2+x^3).
 * https://oeis.org/A077930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077930
 *  Expansion of (1–x)^(-1)/(1+2*x+x^2+x^3).
 *
 *  Recurrence:
 *      a(n) = -a(n-1) + a(n-2) + a(n-4)   for n ≥ 4
 *      a(0)= 1,  a(1)= -1,  a(2)= 2,  a(3)= -3
 *
 *  The function stores computed terms in a static cache so that
 *  repeated calls are O(1).  The cache grows geometrically as needed.
 */

#include <stdlib.h>

/* static cache: a[0] … a[cache_len-1] */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of terms already computed   */
static size_t cache_cap = 0;      /* size of allocated array             */

seq_t a(idx_t n)
{
    /* Return the small pre‑computed values directly. */
    if (n < 4) {
        switch ((uint64_t)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)-1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)-3;
        }
    }

    /* If the cache already contains n, just return it. */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-3;
        cache_len = 4;
    }

    /* Grow the cache if required.  Growth is at least double to keep
     * amortised linear complexity.  The "+10" guard protects against
     * very small increments for the very first expansion. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 10;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t_nm1 = cache[i - 1];
        seq_t t_nm2 = cache[i - 2];
        seq_t t_nm4 = cache[i - 4];
        cache[i] = -t_nm1 + t_nm2 + t_nm4;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

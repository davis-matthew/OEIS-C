/*
 * A225500  Expansion of 1/(1 - x - x^5 + x^6 - x^7 - x^11 + x^12).
 * https://oeis.org/A225500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-5)-a(n-6)+a(n-7)+a(n-11)-a(n-12), a(0)=1
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 86, u64 n <= 170,
 *            u128 n <= 338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* memoised computation of the coefficients of 1/(1 - x - x^5 + x^6 - x^7 - x^11 + x^12) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_size = 0;     /* number of stored entries */

/* ensure the cache can hold at least (n + 1) entries */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1))
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise for n = 0 */
    if (cache == NULL) {
        ensure_capacity(0);
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    /* grow cache to include n */
    ensure_capacity(n);

    /* compute missing values iteratively */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        size_t ii = (size_t)i;
        if (i >= 1) sum += cache[ii - 1];
        if (i >= 5) sum += cache[ii - 5];
        if (i >= 7) sum += cache[ii - 7];
        if (i >= 11) sum += cache[ii - 11];
        if (i >= 6) sum -= cache[ii - 6];
        if (i >= 12) sum -= cache[ii - 12];
        cache[ii] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}

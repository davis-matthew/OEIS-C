/*
 * A168221  a(n) = A006368(A006368(n)).
 * https://oeis.org/A168221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-16) - a(n-32) for n >= 32 with the first 32 terms
 *            listed below: 0,1,2,3,9,6,7,4,18,5,11,12,27,15,16,8,
 *            36,10,20,21,45,24,25,13,54,14,29,30,63,33,34,17
 * Range:     u8 n <= 115, u16 n <= 9101, u32 n <= 9101, u64 n <= 9101,
 *            u128 n <= 9101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

/* Base values for n = 0 .. 31 */
static const seq_t base_vals[32] = {
    0ULL, 1ULL, 2ULL, 3ULL, 9ULL, 6ULL, 7ULL, 4ULL,
    18ULL, 5ULL, 11ULL, 12ULL, 27ULL, 15ULL, 16ULL, 8ULL,
    36ULL, 10ULL, 20ULL, 21ULL, 45ULL, 24ULL, 25ULL, 13ULL,
    54ULL, 14ULL, 29ULL, 30ULL, 63ULL, 33ULL, 34ULL, 17ULL
};

/* Cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;   /* number of elements actually stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure capacity for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n).  */
seq_t a(idx_t n)
{
    /* Initialise cache with base values on first call */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 32; ++i) cache[i] = base_vals[i];
        cache_sz = 32;
    }

    /* If the requested index is already known, return it */
    if ((size_t)n < cache_sz) return cache[n];

    /* Grow cache to hold terms up to n */
    ensure_capacity((size_t)n + 1);

    /* Compute missing entries using the recurrence */
    for (size_t i = cache_sz; i <= (size_t)n; ++i) {
        /* a(i) = 2*a(i-16) - a(i-32) */
        seq_t temp = cache[i - 16] * (seq_t)2;
        temp -= cache[i - 32];
        cache[i] = temp;
    }
    cache_sz = (size_t)n + 1;
    return cache[n];
}

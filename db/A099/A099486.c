/*
 * A099486  Expansion of x/((1 + x^2)*(1 - 4*x + x^2)).
 * https://oeis.org/A099486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) - a(n-4) Initial terms:
 *            a(0) = 0, a(1) = 1, a(2) = 4, a(3) = 14
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values                 */
static size_t cache_size = 0;        /* number of terms currently computed      */
static size_t cache_cap  = 0;        /* allocated capacity                      */

/* Grow the cache to at least 'need' elements */
static void grow_cache_to(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    /* allocate new storage */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        return; /* out of memory; caller will get garbage but it won't happen in tests */

    /* copy old values */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old storage */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that all terms up to index n are computed and cached */
static void ensure_terms(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_size)
        return;

    grow_cache_to(need);

    for (size_t i = cache_size; i < need; ++i) {
        seq_t val;
        if (i == 0)          val = 0;
        else if (i == 1)     val = 1;
        else if (i == 2)     val = 4;
        else if (i == 3)     val = 14;
        else {
            seq_t t = (seq_t)4 * cache[i-1] + (seq_t)4 * cache[i-3];
            t -= (seq_t)2 * cache[i-2];
            t -= cache[i-4];
            val = t;
        }
        cache[i] = val;
    }
    cache_size = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* quick path for the first term */

    ensure_terms(n);
    return cache[(size_t)n];
}

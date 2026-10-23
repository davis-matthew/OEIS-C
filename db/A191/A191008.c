/*
 * A191008  a(n) = (n*3^(n+1)+((5*3^(n+1)+(-1)^(n))/4))/4.
 * https://oeis.org/A191008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 9*a(n-3), a(0)=1, a(1)=5, a(2)=22
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed values      */
static size_t cache_cap = 0;         /* capacity of the cache array   */
static size_t computed = 0;          /* number of entries computed    */

/* Utility: multiply a seq_t by a small integer constant.
   Uses 128‑bit intermediate when seq_t is <= 8 bytes. */
static seq_t mul_const(seq_t x, unsigned int c)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)x * c;
        return (seq_t)tmp;
    } else {
        return x * c;               /* big‑int implementation handles overflow */
    }
}

/* Ensure that the cache can store values up to index 'max_n' (inclusive). */
static void ensure_capacity(idx_t max_n)
{
    size_t needed = (size_t)max_n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed + 16;

    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory – terminate safely */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first three terms. */
static void init_base(void)
{
    if (cache != NULL) return;        /* already initialised */

    size_t cap = 16;                  /* arbitrary small start */
    cache = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!cache) abort();

    cache_cap = cap;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)22;
    computed = 3;                     /* indices 0,1,2 are ready */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_base();                       /* ensure base terms are present */

    /* Fast return for already computed indices */
    if ((size_t)n < computed) {
        return cache[(size_t)n];
    }

    /* Make sure we have room for all indices up to 'n' */
    ensure_capacity(n);

    /* Compute missing terms using the recurrence */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t t = mul_const(cache[i-1], 5);
        t -= mul_const(cache[i-2], 3);
        t -= mul_const(cache[i-3], 9);
        cache[i] = t;
    }

    computed = (size_t)n + 1;          /* all indices up to n are now ready */
    return cache[(size_t)n];
}

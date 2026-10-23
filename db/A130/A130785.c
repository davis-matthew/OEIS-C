/*
 * A130785  Sequence identical to its third differences: a(n+3) =
 *          3a(n+2)-3a(n+1)+2a(n), with a(0)=1, a(1)=4, a(2)=9.
 * https://oeis.org/A130785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130785: recurrence a(n+3)=3*a(n+2)-3*a(n+1)+2*a(n),
   initial values a(0)=1, a(1)=4, a(2)=9. */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms          */
static size_t cache_cap = 0;     /* capacity of the array            */
static size_t cache_size = 0;    /* number of terms currently stored */

/* Ensure the cache can hold indices up to 'n' (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while ((size_t)n >= new_cap)
        new_cap *= 2;

#ifdef __cplusplus
    /* allocate new array with constructors called */
    seq_t *new_cache = new seq_t[new_cap];
#else
    /* plain C allocation */
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif

    if (!new_cache)
        abort();   /* unrecoverable failure */

    /* copy existing values to the new cache */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

#ifdef __cplusplus
    delete [] cache;   /* destruct old array */
#else
    /* In C, old memory will be freed by realloc if it returned a new block,
       otherwise nothing needs to be done. */
    cache = (seq_t *)cache;  /* silence unused warning */
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first three terms if not already done. */
static void init_cache(void)
{
    if (!cache)
    {
        ensure_cache_capacity(2);          /* at least 3 elements needed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache_size = 3;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_size)
        return cache[n];

    ensure_cache_capacity(n);

    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        /* recurrence: a[i] = 3*a[i-1] - 3*a[i-2] + 2*a[i-3] */
        seq_t term1 = cache[i-1] * (seq_t)3;
        seq_t term2 = cache[i-2] * (seq_t)3;
        seq_t term3 = cache[i-3] * (seq_t)2;
        cache[i] = term1 - term2 + term3;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}

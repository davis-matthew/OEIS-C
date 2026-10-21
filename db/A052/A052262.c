/*
 * A052262  Partial sums of A014824.
 * https://oeis.org/A052262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 10*a(n-1) + binomial(n+1,2), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (binomial(n+1,2) = (n+1)*n/2)  */

/* This implementation caches computed values to allow calls in any order. */
 
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;          /* array of a(i) values */
static idx_t  cached_up_to = 0;      /* largest index computed so far */
static size_t cache_capacity = 0;    /* actual allocated size (in elements) */

/* Compute binomial(n+1,2) = (n+1)*n/2 using seq_t arithmetic */
static inline seq_t binom2(idx_t n)
{
    seq_t a = (seq_t)n;                     /* n          */
    seq_t b = (seq_t)(n + 1);               /* n+1        */
    return (a * b) / (seq_t)2;              /* exact division */
}

/* Ensure cache holds values up to index 'req' (inclusive) */
static void ensure_cache(idx_t req)
{
    /* First allocation */
    if (cache == NULL)
    {
        /* Pick an initial capacity, but at least req+1 */
        cache_capacity = (req + 1 > 32 ? req + 1 : 32);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (cache == NULL) exit(1);          /* abort on allocation failure */
        cache[0] = (seq_t)0;                 /* a(0) = 0 */
        cached_up_to = 0;
        return;
    }

    /* Nothing to do if already computed */
    if (req <= cached_up_to)
        return;

    /* Grow capacity if necessary */
    if (req + 1 > cache_capacity)
    {
        size_t new_cap = cache_capacity;
        while (new_cap <= (size_t)(req + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cached_up_to + 1; i <= req; ++i)
    {
        seq_t term = (seq_t)10 * cache[i - 1] + binom2(i);
        cache[i] = term;
    }
    cached_up_to = req;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handles the case n == 0 as well as subsequent indices */
    if (cache == NULL)
        ensure_cache(0);          /* guarantees cache[0] exists */
    else
        ensure_cache(n);
    return cache[n];
}

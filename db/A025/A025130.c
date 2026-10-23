/*
 * A025130  Expansion of 1/((1-x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A025130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (80*12^(n+3)-132*11^(n+3)+55*9^(n+3)-3)/2640 Recurrence:
 *            a(n) = 33*a(n-1) - 371*a(n-2) + 1527*a(n-3) - 1188*a(n-4), with
 *            a(0)=1, a(1)=33, a(2)=718, a(3)=12978
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* cached values of the sequence   */
static idx_t cache_size  = 0;     /* current capacity of cache       */
static idx_t last_computed = 0;   /* highest index that has been computed */

/* Initialise the cache with the first four terms, if necessary */
static void initialise_cache(void)
{
    if (cache != NULL) return;          /* already initialised */

    cache_size = 4;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    if (cache == NULL) abort();          /* caller must never hit OOM */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)33;
    cache[2] = (seq_t)718;
    cache[3] = (seq_t)12978;

    last_computed = 3;                   /* indices 0..3 are now valid */
}

/* Ensure the cache can store indices up to n; grow if necessary */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        initialise_cache();
        return;
    }

    if (cache_size <= n) {
        /* Allocate at least one more element than needed to reduce reallocs */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_size;
    }
}

/* Compute a(n) using the linear recurrence and memoisation */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;          /* safety for unsigned wrap */

    ensure_capacity(n);

    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val  = (seq_t)33  * cache[i-1];
        val       -= (seq_t)371 * cache[i-2];
        val       += (seq_t)1527 * cache[i-3];
        val       -= (seq_t)1188 * cache[i-4];
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}

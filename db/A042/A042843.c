/*
 * A042843  Denominators of continued fraction convergents to sqrt(952).
 * https://oeis.org/A042843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A042843 – Denominators of continued fraction convergents to sqrt(952)
 *
 * Recurrence:
 *   a(n) = 23326*a(n-8) - a(n-16)   for n > 15
 * Initial values a(0) … a(15) are:
 *   1, 1, 6, 7, 48, 55, 323, 378, 23003, 23381,
 *   139908, 163289, 1119642, 1282931, 7534297, 8817228
 *
 * The function below caches the terms in a static array so it can answer
 * any call for n ≥ 0 in O(n) time with O(n) memory.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Initial 16 terms of the sequence */
static const seq_t initial[16] = {
    (seq_t)1,          /* a(0) */
    (seq_t)1,          /* a(1) */
    (seq_t)6,          /* a(2) */
    (seq_t)7,          /* a(3) */
    (seq_t)48,         /* a(4) */
    (seq_t)55,         /* a(5) */
    (seq_t)323,        /* a(6) */
    (seq_t)378,        /* a(7) */
    (seq_t)23003,      /* a(8) */
    (seq_t)23381,      /* a(9) */
    (seq_t)139908,     /* a(10) */
    (seq_t)163289,     /* a(11) */
    (seq_t)1119642,    /* a(12) */
    (seq_t)1282931,    /* a(13) */
    (seq_t)7534297,    /* a(14) */
    (seq_t)8817228     /* a(15) */
};

/* Cache for computed terms */
static seq_t *cache   = NULL;  /* pointer to the allocated array */
static size_t cache_size = 0;   /* number of filled entries in cache */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* Ensure that the cache can hold at least 'cap' elements */
static void ensure_capacity(size_t cap)
{
    if (cap <= cache_cap)
        return;

    size_t newcap = (cache_cap == 0) ? 16 : cache_cap;
    while (newcap < cap)
        newcap <<= 1;

    /* realloc returns void*, must be cast to seq_t* for arbitrary‑precision types */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */

    cache = tmp;
    cache_cap = newcap;
}

/*
 * External function required by the build system.
 * Computes the n-th term of sequence A042843.
 */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing; safe for the expected range */
    size_t idx = (size_t)n;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        ensure_capacity(16);
        for (size_t i = 0; i < 16; ++i)
            cache[i] = initial[i];
        cache_size = 16;
    }

    /* Compute missing terms up to idx (inclusive) */
    if (idx >= cache_size) {
        ensure_capacity(idx + 1);
        for (size_t i = cache_size; i <= idx; ++i) {
            /* For i ≥ 16, a(i-8) and a(i-16) are already cached */
            cache[i] = (seq_t)23326 * cache[i - 8] - cache[i - 16];
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}

/*
 * A124023  G.f.: 1/(1-2*x-6*x^2+4*x^3).
 * https://oeis.org/A124023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=10,
   a(n)=2*a(n-1)+6*a(n-2)-4*a(n-3) for n>=3
   (derived from generating function 1/(1-2*x-6*x^2+4*x^3)) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* stored values */
static size_t  cap       = 0;      /* current capacity of cache */
static size_t  filled    = 0;      /* number of valid entries in cache */

/* Grow the cache to hold at least indices 0 .. n */
static void grow_cache(idx_t n)
{
    size_t newcap = (cap == 0) ? 4 : cap * 2;
    while (newcap <= (size_t)n) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();               /* unrecoverable: out of memory */
    cache = tmp;
    cap   = newcap;
}

/* Return the n-th term of sequence A124023 (0‑based). */
seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache == NULL) {
        cap    = 4;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        /* cache[3] is left uninitialized until needed */
        filled = 3;    /* indices 0..2 are valid */
    }

    /* If the requested index is already computed, return it directly */
    if ((idx_t)filled > n) return cache[n];

    /* Ensure we have enough room in the cache */
    if ((idx_t)cap <= n) grow_cache(n);

    /* Compute missing values using the recurrence */
    for (idx_t i = (idx_t)filled; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        t1 *= (seq_t)2;             /* 2 * a(i-1) */
        seq_t t2 = cache[i-2];
        t2 *= (seq_t)6;             /* 6 * a(i-2) */
        seq_t t3 = cache[i-3];
        t3 *= (seq_t)4;             /* 4 * a(i-3) */
        cache[i] = t1 + t2 - t3;    /* recurrence */
    }
    filled = (size_t)n + 1;   /* all indices up to n are now valid */

    return cache[(size_t)n];
}

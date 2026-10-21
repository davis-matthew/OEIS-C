/*
 * A088723  Numbers k with at least one divisor d>1 such that d+1 also divides
 *          k.
 * https://oeis.org/A088723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088723 – numbers k for which there exists a divisor d>1 with d+1 also
   dividing k.  Such k must be a multiple of d*(d+1) for some d≥2.  Hence
   the sequence is exactly the union of all arithmetic progressions
   {t·d·(d+1) | t≥1, d≥2}.  The function below keeps a static cache of
   the generated terms and expands it exponentially until the requested
   index n is available.  It works for any seq_t (unsigned integer or
   arbitrary‑precision class) and for any idx_t (unsigned integer). */

/* compile‑time includes that are valid in both C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Static data shared by a() and its helper                            */
/* --------------------------------------------------------------------- */
static idx_t   cache_len  = 0;   /* number of stored terms          */
static seq_t * cache      = NULL;/* array of stored terms           */
static uint64_t sieve_limit = 0;/* current sieve upper bound       */

/* --------------------------------------------------------------------- */
/*  Rebuild the cache up to a given limit                               */
/* --------------------------------------------------------------------- */
static void rebuild_cache(uint64_t limit)
{
    /* Allocate a temporary mark array */
    uint8_t *mark = (uint8_t*)malloc(limit + 1);
    if (!mark) exit(1);                 /* abort on allocation failure   */

    for (size_t i = 0; i <= limit; ++i) mark[i] = 0;

    /* Mark every multiple of d*(d+1) for d>=2 */
    for (uint64_t d = 2; ; ++d)
    {
        uint64_t step = d * (d + 1);
        if (step > limit) break;
        for (uint64_t m = step; m <= limit; m += step)
            mark[m] = 1;
    }

    /* Count how many marks we have */
    uint32_t count = 0;
    for (uint64_t i = 1; i <= limit; ++i)
        if (mark[i]) ++count;

    /* Allocate the new cache and fill it */
    seq_t *new_cache = (seq_t*)malloc((size_t)count * sizeof(seq_t));
    if (!new_cache)
    {
        free(mark);
        exit(1);
    }

    uint32_t pos = 0;
    for (uint64_t i = 1; i <= limit; ++i)
        if (mark[i]) new_cache[pos++] = (seq_t)i;

    /* Replace the old cache */
    if (cache) free(cache);
    cache      = new_cache;
    cache_len  = count;

    free(mark);
}

/* --------------------------------------------------------------------- */
/*  Public function requested by the build system                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence starts at index 1; behaviour for n==0 is irrelevant */
    if (n == 0) return (seq_t)0;

    /* Grow the sieve until we have at least n terms */
    if (cache_len < n)
    {
        if (sieve_limit == 0) sieve_limit = 10;           /* initial bound */
        while (cache_len < n)
        {
            sieve_limit *= 2;                           /* exponential growth */
            rebuild_cache(sieve_limit);
        }
    }
    return cache[n - 1];    /* zero‑based array indexing */
}

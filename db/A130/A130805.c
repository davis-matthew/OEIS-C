/*
 * A130805  a(1)=1. The n-th run (after the initial 1) consists of (a(n)+n)
 *          repeated a(n) times.
 * https://oeis.org/A130805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130805: a(1)=1.  
   For r ≥ 1, the r‑th run (after the initial 1) consists of
   a(r)+r repeated a(r) times, filling subsequent terms. */

#include <stdlib.h>

/* Static cache */
static seq_t *cache = NULL;      /* 1‑based indexing, cache[1] = a(1) */
static size_t cache_cap = 0;     /* capacity of cache array (elements 0..cap-1) */
static size_t pos = 2;           /* next free index to store (initially a[2]) */
static idx_t next_run = 1;       /* current run index to process */

/* Ensure cache can hold at least 'needed' elements (indices 0..needed‑1). */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A130805 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */

    /* Initialise cache on first call */
    if (!cache)
    {
        ensure_capacity(4);     /* indices 0..3 will be used */
        cache[1] = (seq_t)1;    /* a(1) = 1 */
        /* pos and next_run are already set to initial values */
    }

    /* Grow cache to accommodate index n */
    if (n >= (idx_t)cache_cap) ensure_capacity((size_t)n + 1);

    /* Generate runs until we have a[n] */
    while (pos <= (size_t)n)
    {
        seq_t val      = cache[(size_t)next_run];       /* a(next_run)   */
        size_t run_len = (size_t)val;                   /* repetitions   */
        seq_t run_val  = val + (seq_t)next_run;          /* value in run  */

        size_t start = pos;
        size_t end   = start + run_len;                 /* exclusive end */

        if (end >= cache_cap) ensure_capacity(end + 1);

        for (size_t i = start; i < end; ++i) cache[i] = run_val;

        pos = end;
        ++next_run;
    }

    return cache[(size_t)n];
}

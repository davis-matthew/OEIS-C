/*
 * A160239  Number of "ON" cells in a 2-dimensional cellular automaton
 *          ("Fredkin's Replicator") evolving according to the rule that a
 *          cell is ON in a given generation if and only if there was an odd
 *          number of ON cells among the eight nearest neighbors in the
 *          preceding generation, starting with one ON cell.
 * https://oeis.org/A160239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(0)=1; * a(2t)=a(t); * a(4t+1)=8*a(t); *
 *            a(4t+3)=2*a(2t+1)+8*a(t) (t>=0)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS entry A160239 */
#include <stdlib.h>

/* static memoisation cache */
static seq_t *cache = NULL;        /* pointer to array of seq_t */
static size_t cache_size = 0;      /* current size of cache */

static void ensure_cache(idx_t n)
{
    /* n must fit in size_t; the sequence is only tested for small n (~10⁴) */
    if ((size_t)n < cache_size) return;

    size_t new_size = cache_size ? cache_size : 16;
    while ((size_t)n >= new_size) new_size <<= 1;

    /* realloc may return a different pointer; we cast it to seq_t* for C/C++ */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();       /* out of memory */

    /* initialise new elements to zero (unused value) */
    for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;

    cache = new_cache;
    cache_size = new_size;
}

/* recursive helper with memoisation */
static seq_t calc(idx_t n)
{
    if (n == 0) return 1;                /* base case */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];  /* already computed */

    seq_t v;
    if ((n & 1) == 0)                /* even: a(2t)=a(t) */
    {
        v = calc(n >> 1);
    }
    else if ((n & 3) == 1)           /* 4t+1: a(4t+1)=8*a(t) */
    {
        v = 8 * calc((n - 1) >> 2);
    }
    else                             /* 4t+3: a(4t+3)=2*a(2t+1)+8*a(t) */
    {
        v = 2 * calc((n - 1) >> 1) + 8 * calc((n - 3) >> 2);
    }

    cache[n] = v;                     /* cache for future calls */
    return v;
}

/* public API required by the harness */
seq_t a(idx_t n)
{
    return calc(n);
}

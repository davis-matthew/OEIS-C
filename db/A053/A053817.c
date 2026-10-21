/*
 * A053817  a(0)=1, a(n) = n*(a(n-1) + n).
 * https://oeis.org/A053817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(n) = n * (a(n-1) + n) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values for memoisation. */
static seq_t *cache   = NULL;
static idx_t  cache_n = 0;

/* Compute a(n) using the recurrence and memoise results. */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0). */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* unlikely */
        cache[0] = (seq_t)1;
        cache_n  = 1;
    }

    /* Extend cache up to the requested index. */
    while (cache_n <= n) {
        idx_t   i   = cache_n;                 /* next index to compute */
        seq_t  prev = cache[i-1];              /* a(i-1) */
        seq_t  term = (seq_t)i * (prev + (seq_t)i); /* a(i) = i*(prev + i) */

        seq_t *tmp = (seq_t *)realloc(cache, (cache_n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* unlikely */
        cache = tmp;
        cache[i]  = term;
        cache_n++;
    }

    return cache[n];
}

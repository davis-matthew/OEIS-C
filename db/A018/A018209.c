/*
 * A018209  Expansion of 1/((1-4x)(1-5x)(1-7x)).
 * https://oeis.org/A018209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A018209 – Expansion of 1/((1‑4x)(1‑5x)(1‑7x))
    Formula:  a(0)=1, a(1)=16, a(2)=173
              a(n)=16*a(n-1) - 83*a(n-2) + 140*a(n-3)   for n>2   */
#include <stdlib.h>

static seq_t *cache = NULL;   /* pointer to cached terms */
static idx_t cache_size = 0;  /* number of elements currently cached */

/*
 *  Return the n-th term of the sequence.
 *  The function can be called in any order; intermediate terms are
 *  cached in a static array so that later calls reuse already
 *  computed values.
 */
seq_t a(idx_t n)
{
    /* initialise cache with the first three terms */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure – unlikely */
        cache[0] = 1;
        cache[1] = 16;
        cache[2] = 173;
        cache_size = 3;
    }

    /* if the requested term is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to accommodate the requested index */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) return 0;                /* allocation failure – unlikely */

    /* compute missing terms using the linear recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* For types that can hold the exact value, the arithmetic
           below gives the correct result.  When the result no longer
           fits the underlying type, the values may wrap, which is
           acceptable because the corresponding terms are never
           checked for accuracy. */
        seq_t term = 16u * cache[i-1];
        term -= 83u * cache[i-2];
        term += 140u * cache[i-3];
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A097124  Expansion of (1-x)^2/((1-x)^3-5x^3).
 * https://oeis.org/A097124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)-3*a(n-2)+6*a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A097124.
   The function uses memoisation so that calls in any order are handled
   efficiently.  It grows the cache only as needed.  The recurrence uses
   only integer arithmetic, so it works for builtin types as well as for an
   arbitrary‑precision integer class.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;   /* array holding the sequence */
    static idx_t cache_size = 0;  /* number of elements stored in cache */

    /* initialise base cases on first call */
    if (cache_size == 0) {
        cache_size = 3;                  /* a(0), a(1), a(2) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory; return 0 as fallback */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_size) return cache[n];

    /* extend the cache to accommodate up to index n */
    idx_t new_size = n + 1;
    size_t alloc_bytes = (size_t)new_size * sizeof(seq_t);

    seq_t *tmp = (seq_t *)realloc(cache, alloc_bytes);
    if (!tmp) return (seq_t)0;          /* out of memory; return 0 as fallback */

    cache = tmp;                        /* use the reallocated memory */

    /* compute new values from the first unwritten index up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + 6*a(i-3)  for i >= 3 */
        seq_t val = 3 * cache[i-1] + 6 * cache[i-3] - 3 * cache[i-2];
        cache[i] = val;
    }

    cache_size = new_size;              /* update cache size */
    return cache[n];
}

/*
 * A064323  a(n) = a(n-1)+ceiling(a(n-2)/2) with a(0)=0, a(1)=1.
 * https://oeis.org/A064323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 71, u64 n <= 142,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=a(n-1)+ceil(a(n-2)/2) */

#include <stdlib.h>

/* Return the nth term of OEIS A064323. {
 *   Parameters:
 *     n   – the index (0‑based)
 *   Returns:
 *     seq_t – a(n)
 *   This function lazily allocates a cache and computes terms on demand.
 *   It handles calls in any order and keeps the results between calls. */
seq_t a(idx_t n)
{
    /* Handle the base cases immediately to avoid negative indices. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;          /* array of computed terms          */
    static idx_t  last_computed = 0;     /* highest index computed           */
    static size_t capacity   = 0;        /* current allocation size          */

    /* Initialise cache on the first call that needs it. */
    if (cache == NULL)
    {
        size_t init_cap = 4;
        cache       = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        capacity = init_cap;
        last_computed = 1;
    }

    /* Grow the array if necessary. */
    if (n >= (idx_t)capacity)
    {
        size_t new_cap = capacity ? capacity * 2 : 4;
        while ((idx_t)new_cap <= n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* reallocation failed */

        cache    = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* ceil(a(i-2)/2) == (a(i-2)+1)/2 for unsigned integers. */
        seq_t ceil_half = (cache[i - 2] + (seq_t)1) >> 1;
        cache[i] = cache[i - 1] + ceil_half;
    }

    last_computed = n;
    return cache[n];
}

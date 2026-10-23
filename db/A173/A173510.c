/*
 * A173510  a(n) = a(n-1) + a(n-2) - floor( a(n-1)/2 ).
 * https://oeis.org/A173510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=2, a(1)=1; a(n)=a(n-1)+a(n-2)-floor(a(n-1)/2) for n>=2 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated array size */
    static size_t sz    = 0;   /* number of valid elements in cache */

    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    /* Initialise cache on first call that needs it */
    if (cache == NULL)
    {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();               /* fatal error: allocation failed */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        sz = 2;                                   /* we now have two values */
    }

    /* If we already have the requested value, return it */
    if (n < sz) return cache[n];

    /* Grow the array if necessary */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap ? cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();                   /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i)
    {
        seq_t prev      = cache[i - 1];
        seq_t prevprev  = cache[i - 2];
        seq_t next      = prev + prevprev - (prev / (seq_t)2); /* floor(prev/2) */
        cache[i] = next;
    }

    sz = n + 1;            /* new number of valid entries */
    return cache[n];
}

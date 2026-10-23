/*
 * A164913  Expansion of x/(1-9*x-11*x^2+10*x^3).
 * https://oeis.org/A164913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164913 (OEIS offset = 1)
 * Recurrence: a(n) = 9*a(n‑1) + 11*a(n‑2) – 10*a(n‑3),  n ≥ 4
 * Initial values (for n = 1,2,3):  a(1)=1, a(2)=9, a(3)=92
 * The function is memoised; it works for any seq_t/idx_t pair
 * provided by the build system. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset is 1 – for n==0 return 0 (unused) */
    if (n == 0)  return (seq_t)0;

    /* static cache: indexed from 1 */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0;          /* largest index that has been filled */
    static size_t cap     = 0;          /* size of allocated array */

    /* insure capacity to hold indices up to n (and at least 3) */
    size_t needed = (size_t)n + 1;
    if (needed < 4) needed = 4;           /* 1,2,3 are needed */

    if (!cache) {
        /* first allocation */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();              /* out of memory */
        cap = needed;
    } else if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;
        cap = needed;
    }

    /* initialise the first three terms if not yet done */
    if (cached < 3) {
        /* we know cache[0] is unused; only 1‑based indices matter */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        /* initialise a(3) directly using the given value */
        cache[3] = (seq_t)92;
        cached = 3;
    }

    /* if the requested term is already in cache, return it */
    if (n <= cached)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* use wider arithmetic if seq_t is small to avoid undefined behaviour
           but normal unsigned wrap‑around is acceptable for values that
           fit in seq_t. */
        seq_t term = (seq_t)(9  * cache[i-1]
                           + 11 * cache[i-2]
                           - 10 * cache[i-3]);
        cache[i] = term;
    }
    cached = n;
    return cache[n];
}

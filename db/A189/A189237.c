/*
 * A189237  Expansion of (5-12*x-9*x^2+8*x^3+x^4)/(1-3*x-3*x^2+4*x^3+x^4-x^5).
 * https://oeis.org/A189237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189237
 *  a(0) = 5, a(1) = 3, a(2) = 15, a(3) = 42, a(4) = 155
 *  a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5)   (n >= 5)
 *
 *  Implements the linear recurrence using a memoised static cache.
 *  Works for any seq_t (4/8/16/32/64/128‑bit unsigned or arbitrary precision).
 *  The function is O(n) for the greatest argument seen so far and is
 *  independent of the order of calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;     /* allocated size of cache   */
    static idx_t   len    = 0;     /* number of entries stored  */

    /* first invocation – create cache and store first 5 terms   */
    if (!cache)
    {
        cap = 16;               /* initial allocation size   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure fallback */

        cache[0] = (seq_t)5;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)155;
        len = 5;
    }

    /* if the requested value is already cached, return it */
    if (n < len)
        return cache[n];

    /* ensure enough room in the cache */
    if ((size_t)n >= cap)
    {
        while (cap <= (size_t)n)
            cap <<= 1;          /* double until large enough   */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure fallback */
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = len; i <= n; ++i)
    {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];

        cache[i] = 3 * a1 + 3 * a2 - 4 * a3 - a4 + a5;
    }

    len = n + 1;                 /* we now have values 0 .. n  */
    return cache[n];
}

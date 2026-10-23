/*
 * A120748  Expansion of x^2*(1 + 2*x - x^2)/(1 - x - 3*x^2 - x^3 + x^4).
 * https://oeis.org/A120748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) - a(n-4) Initial values:
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=5
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Short-circuit for the offset and zero (or negative) indices. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache of computed terms (1‑indexed). */
    static seq_t *cache = NULL;     /* points to cache[0] ... cache[cache_cap-1] */
    static idx_t cache_cap = 0;     /* size of allocated memory (in seq_t units) */
    static idx_t cached_upto = 0;   /* highest n that has been computed */

    /* Ensure we have room for up to n inclusive (plus one for 1‑index). */
    if (cache_cap <= n)
    {
        /* Grow: double the capacity until it fits, then reallocate. */
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize the first four values if they have not been done yet. */
    if (cached_upto < 4)
    {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cached_upto = (cached_upto < 4) ? 4 : cached_upto;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cached_upto + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 3*a(i-2) + a(i-3) - a(i-4) */
        seq_t term = cache[i - 1] + (seq_t)3 * cache[i - 2] + cache[i - 3] - cache[i - 4];
        cache[i] = term;
    }
    cached_upto = n;

    return cache[n];
}

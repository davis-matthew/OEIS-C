/*
 * A192979  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4), with a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=9
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent explicit formula:
   a(n) = Fibonacci(n+3) + Lucas(n+2) - 2*(n+2). */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Small n handled directly to avoid Caches */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)4;
            default: /* n==3 */ return (seq_t)9;
        }
    }

    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cap    = 0;      /* current capacity of cache   */
    static idx_t last    = 3;      /* index of last computed value */

    /* First allocation and initialization of the base values */
    if (cache == NULL) {
        cap   = (size_t)((n + 1 > 8) ? (n + 1) : 8);   /* start with at least 8 slots */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                    /* out‑of‑memory */

        /* Insert the four known terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;

        /* Zero‑initialize remaining slots for safety */
        for (size_t i = 4; i < cap; ++i) cache[i] = 0;
    }

    /* Extend cache if necessary */
    if ((idx_t)(cap - 1) < n) {
        size_t newcap = cap;
        while ((idx_t)(newcap - 1) < n) newcap <<= 1;   /* double until large enough */

        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        cache = tmp;

        /* Initialize newly allocated entries */
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* If already computed, just return the value */
    if (n <= last) return cache[n];

    /* Compute the values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)3 * cache[(size_t)(i - 1)]
          - (seq_t)2 * cache[(size_t)(i - 2)]
          -          cache[(size_t)(i - 3)]
          +          cache[(size_t)(i - 4)];
    }
    last = n;
    return cache[(size_t)n];
}

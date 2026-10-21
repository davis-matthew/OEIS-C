/*
 * A052670  Expansion of e.g.f. x^2/(1-4*x).
 * https://oeis.org/A052670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 2, a(n) = 4 * n * a(n-1) (for n > 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for sequence A052670.
   Uses memoization to avoid recomputing values.
   Works for any seq_t that supports standard integer arithmetic. */
seq_t a(idx_t n)
{
    /* static cache: allocated on first call */
    static seq_t *cache   = NULL;
    static idx_t   cap    = 0;
    static idx_t   sz     = 0;   /* number of valid elements in cache */

    /* Ensure cache is initialized with base cases a(0)=0, a(1)=0, a(2)=2 */
    if (cache == NULL) {
        /* Start with a small capacity that can hold at least 3 elements */
        cap = 4;                 /* arbitrary small power of two >= 3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        sz = 3;
    }

    /* Expand cache if needed */
    if (n >= sz) {
        /* Find new capacity (next power of 2 > n) */
        idx_t newCap = cap;
        if (newCap == 0) newCap = 1;
        while (newCap <= n)
            newCap <<= 1;

        cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        cap   = newCap;

        /* Compute missing terms up to n */
        for (idx_t i = sz; i <= n; ++i) {
            if (i < 2) {
                cache[i] = (seq_t)0;          /* a(0) = a(1) = 0 */
            } else if (i == 2) {
                cache[i] = (seq_t)2;          /* a(2) = 2 */
            } else {
                /* recurrence: a(i) = 4 * i * a(i-1) */
                cache[i] = cache[i-1] * ((seq_t)4 * (seq_t)i);
            }
        }
        sz = n + 1;
    }

    return cache[n];
}

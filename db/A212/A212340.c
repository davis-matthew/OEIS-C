/*
 * A212340  G.f.: 1/(1-x-x^2-2*x^3-5*x^4).
 * https://oeis.org/A212340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) + 5*a(n-4), initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_computed = (size_t)-1;   /* last index computed */

    /* Handle first four terms directly. */
    if (n < 4) {
        static const seq_t init[4] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)5};
        return init[n];
    }

    /* Allocate initial cache if not yet done. */
    if (!cache) {
        cache_cap = 8;  /* start with a small capacity and grow as needed */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                     /* allocation failure */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 5;
        last_computed = 3;
    }

    /* Ensure cache is large enough for the requested index. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any new terms up to n.  Use the recurrence relation. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + 2*cache[i-3] + 5*cache[i-4];
    }
    last_computed = n;

    return cache[n];
}

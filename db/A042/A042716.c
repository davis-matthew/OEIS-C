/*
 * A042716  Numerators of continued fraction convergents to sqrt(888).
 * https://oeis.org/A042716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 298*a(n-4) – a(n-8) a(0)=29, a(1)=30, a(2)=119, a(3)=149,
 *            a(4)=8761, a(5)=8910, a(6)=35491, a(7)=44401
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 7;                      /* largest index memoized */
    static const seq_t init[8] = {29, 30, 119, 149,
                                  8761, 8910, 35491, 44401};

    /* Base cases covered directly for speed */
    if (n < 8) return init[n];

    /* First allocation for the memoization cache */
    if (!cache) {
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new values up to n incrementally */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)298 * cache[i-4] - cache[i-8];
    }
    last = (size_t)n;

    return cache[n];
}

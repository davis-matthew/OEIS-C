/*
 * A017841  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10).
 * https://oeis.org/A017841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 51, u32 n <= 96, u64 n <= 184,
 *            u128 n <= 361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10) for n>9
   Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0, a(5)=a(6)=a(7)=a(8)=a(9)=1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;               /* current allocated size */
    static idx_t cached_max = (idx_t)-1; /* highest n that has been computed */

    /* initialise cache up to index 9 on first call */
    if (cache == NULL) {
        cap = 10; /* indices 0..9 inclusive */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cap; ++i)
            cache[i] = 0;
        cache[0] = 1;
        cache[5] = cache[6] = cache[7] = cache[8] = cache[9] = 1;
        cached_max = 9;
    }

    /* if n is already computed, return it */
    if (n <= cached_max)
        return cache[(size_t)n];

    /* grow cache if needed */
    if (n >= cap) {
        /* double until cap > n */
        while (cap <= (size_t)n)
            cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute new values up to n using recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* i > 9 because cached_max >= 9, so all indices below i are valid */
        sum += cache[(size_t)(i - 5)];
        sum += cache[(size_t)(i - 6)];
        sum += cache[(size_t)(i - 7)];
        sum += cache[(size_t)(i - 8)];
        sum += cache[(size_t)(i - 9)];
        sum += cache[(size_t)(i - 10)];
        cache[(size_t)i] = sum;
    }
    cached_max = n;

    return cache[(size_t)n];
}

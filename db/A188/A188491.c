/*
 * A188491  Number of permutations p on the set [n] with the properties that
 *          abs(p(i)-i) <= 3 for all i, p(1) <= 3, and p(4) >= 2.
 * https://oeis.org/A188491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by a 13‑term linear recurrence a(n) = a(n-1) +
 *            3*a(n-2) + 3*a(n-3) + 13*a(n-4) + 21*a(n-5) + 19*a(n-6) +
 *            3*a(n-7) - 7*a(n-8) - 9*a(n-9) - 5*a(n-10) - 3*a(n-11) -
 *            3*a(n-12) - a(n-13) (n ≥ 13) Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=14, a(5)=48, a(6)=152, a(7)=476, a(8)...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache   = NULL;    /* array of a(k) for k < cache_size */
    static size_t cache_size = 0;    /* number of values currently stored */
    static size_t cache_cap  = 0;    /* allocated capacity of the array */

    /* First 13 terms, needed to start the recurrence */
    const seq_t init[13] = {
        (seq_t)0,   /* a(0) */
        (seq_t)1,   /* a(1) */
        (seq_t)2,   /* a(2) */
        (seq_t)6,   /* a(3) */
        (seq_t)14,  /* a(4) */
        (seq_t)48,  /* a(5) */
        (seq_t)152, /* a(6) */
        (seq_t)476, /* a(7) */
        (seq_t)1425,/* a(8) */
        (seq_t)4340,/* a(9) */
        (seq_t)13288,/* a(10) */
        (seq_t)40852,/* a(11) */
        (seq_t)125124/* a(12) */
    };

    /* Initialise cache on the first call with the 13 known values */
    if (cache == NULL) {
        cache_cap = 64;                    /* arbitrary initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_size = 13;
    }

    /* Grow the cache if the requested index is outside current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;   /* realloc failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already available, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Coefficients of the recurrence (see comment above). */
    const int coeff[13] = { 1, 3, 3, 13, 21, 19, 3,
                           -7, -9, -5, -3, -3, -1 };

    /* Compute missing terms up to and including a(n) */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 13; ++j) {
            seq_t prev = cache[i - 1 - (size_t)j];
            int c = coeff[j];
            if (c > 0)
                sum += (seq_t)c * prev;
            else
                sum -= (seq_t)(-c) * prev;
        }
        cache[i] = sum;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}

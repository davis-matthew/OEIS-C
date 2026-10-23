/*
 * A025887  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-8) + a(n-9) - a(n-13) - a(n-14) - a(n-17) +
 *            a(n-22) with a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0
 * Range:     u8 n <= 417, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* persistent cache of computed values */
    static seq_t *cache = NULL;
    static size_t cached = 0;    /* number of entries already filled (cache[0..cached-1]) */
    static size_t capacity = 0;  /* allocated size of cache */

    /* initialise cache with a[0] = 1 */
    if (cache == NULL) {
        capacity = 16;                      /* start with a reasonable block */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure => zero */
        cache[0] = (seq_t)1;
        cached = 1;
    }

    /* expand cache if needed */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
            if (new_cap == 0) { /* overflow guard */
                new_cap = (size_t)n + 1;
                break;
            }
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* realloc failure => zero */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing values lazily (any call order allowed) */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        seq_t pos = 0, neg = 0;

        if (i >= 5)  pos += cache[i - 5];
        if (i >= 8)  pos += cache[i - 8];
        if (i >= 9)  pos += cache[i - 9];
        if (i >= 13) neg += cache[i - 13];
        if (i >= 14) neg += cache[i - 14];
        if (i >= 17) neg += cache[i - 17];
        if (i >= 22) pos += cache[i - 22];   /* the final + term */

        cache[i] = pos - neg;
    }
    cached = (size_t)n + 1;

    return cache[n];
}

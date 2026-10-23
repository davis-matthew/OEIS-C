/*
 * A109784  Expansion of (x^2-x-1)*(x^3-x^2+x-2) /
 *          ((x-1)*(2*x-1)*(x^2+x+1)*(x^2+1)).
 * https://oeis.org/A109784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - 2*a(n-4) + a(n-5) -
 *            2*a(n-6) for n>5 with initial values a(0)=2, a(1)=5, a(2)=6,
 *            a(3)=14, a(4)=31, a(5)=59
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms already stored */
    static idx_t capacity = 0;

    /* --- initialize first six terms on first call -------------------------------- */
    if (cache == NULL) {
        capacity = 8;  /* initial capacity, will grow if needed */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;  /* a(0) */
        cache[1] = (seq_t)5;  /* a(1) */
        cache[2] = (seq_t)6;  /* a(2) */
        cache[3] = (seq_t)14; /* a(3) */
        cache[4] = (seq_t)31; /* a(4) */
        cache[5] = (seq_t)59; /* a(5) */
        cached = 6;          /* terms 0..5 stored */
    }

    /* if the requested term is already computed, return it */
    if (n < cached)
        return cache[n];

    /* enlarge the cache if the requested index does not fit yet */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        if (new_cap < capacity * 2)
            new_cap = capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t val =
            2 * cache[i - 1] - cache[i - 2]
          + 3 * cache[i - 3] - 2 * cache[i - 4]
          + cache[i - 5]   - 2 * cache[i - 6];
        cache[i] = val;
    }
    cached = n + 1;
    return cache[n];
}

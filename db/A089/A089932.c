/*
 * A089932  G.f.: (1-x)^5/((-1+4*x-3*x^2+x^3)*(1-3*x+x^2)*(-1+2*x)).
 * https://oeis.org/A089932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A089932
    G.f.: (1-x)^5/((-1+4x-3x^2+x^3)*(1-3x+x^2)*(-1+2x))
    Recurrence (for n ≥ 6):
        a(n) = 9*a(n−1) − 30*a(n−2) + 46*a(n−3) − 34*a(n−4) + 13*a(n−5) − 2*a(n−6)
    Initial values:
        a(0)=1, a(1)=4, a(2)=16, a(3)=60, a(4)=215, a(5)=747
 */

#include <stdlib.h>
#include <string.h>

/* The function must be compiled with C11 or C++17 support.
   The types idx_t and seq_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t last_computed = 0;/* highest index computed */

    /* Ensure the cache is big enough to hold the first six terms */
    if (cache == NULL) {
        cache_size = 6; /* at least 6 terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0; /* allocation failed – unlikely */

        /* initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)60;
        cache[4] = (seq_t)215;
        cache[5] = (seq_t)747;
        last_computed = 5;
    }

    /* Extend cache if required */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing values up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Separate positive and negative contributions to avoid underflow */
            seq_t pos = (seq_t)9  * cache[i-1] + (seq_t)46 * cache[i-3] + (seq_t)13 * cache[i-5];
            seq_t neg = (seq_t)30 * cache[i-2] + (seq_t)34 * cache[i-4] + (seq_t)2  * cache[i-6];
            cache[i] = pos - neg;        /* guaranteed to be non‑negative for n ≥ 6 */
        }
        last_computed = n;
    }

    return cache[n];
}

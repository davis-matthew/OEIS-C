/*
 * A116709  Number of permutations of length n which avoid the patterns 2341,
 *          4213.
 * https://oeis.org/A116709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 25*a(n-3) - 10*a(n-4) + 2*a(n-5)
 *            for n>5 Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=22, a(5)=86
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */

    static seq_t *cache = NULL;            /* cache[0] holds a(1) */
    static size_t cache_size = 0;          /* number of computed terms */
    static size_t cache_cap   = 0;          /* allocated capacity */

    /* initialize cache with the first five terms once */
    if (!cache_size) {
        size_t init_cap = 5;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)6;   /* a(3) */
        cache[3] = (seq_t)22;  /* a(4) */
        cache[4] = (seq_t)86;  /* a(5) */
        cache_size = 5;
    }

    /* return cached value if already computed */
    if ((idx_t)cache_size >= n) {
        return cache[n - 1];
    }

    /* expand cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (should not happen in tests) */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t k = (idx_t)cache_size; k < n; ++k) {
        /* terms are 0‑based: cache[k] corresponds to a(k+1) */
        /* use positive part minus negative part to avoid underflow */
        seq_t pos = (seq_t)8  * cache[k - 1] +
                    (seq_t)25 * cache[k - 3] +
                    (seq_t)2  * cache[k - 5];

        seq_t neg = (seq_t)22 * cache[k - 2] +
                    (seq_t)10 * cache[k - 4];

        cache[k] = pos - neg;
    }
    cache_size = n;
    return cache[n - 1];
}

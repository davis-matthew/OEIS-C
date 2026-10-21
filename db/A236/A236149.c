/*
 * A236149  Number of (n+1) X (2+1) 0..2 arrays colored with the sets of
 *          distinct values in every 2 X 2 subblock.
 * https://oeis.org/A236149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 19*a(n-2) + 24*a(n-3) - a(n-4) - 22*a(n-5) -
 *            11*a(n-6) - 2*a(n-7) - 2*a(n-8) Initial terms: a(1)=414,
 *            a(2)=2502, a(3)=16128, a(4)=104436, a(5)=676980, a(6)=4389660,
 *            a(7)=28462080, a(8)=184549536
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;      /* array of already computed terms */
    static size_t cap = 0;           /* allocated capacity of cache */
    static idx_t last_computed = 0;  /* number of terms computed so far */

    /* first 8 terms, stored in static array */
    static const seq_t init[8] = {
        (seq_t)414,
        (seq_t)2502,
        (seq_t)16128,
        (seq_t)104436,
        (seq_t)676980,
        (seq_t)4389660,
        (seq_t)28462080,
        (seq_t)184549536
    };

    if (n == 0) return 0;  /* undefined, but avoid underflow */

    /* ensure cache has at least 8 slots */
    if (!cache) {
        cap = n > 8 ? (size_t)n : 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        last_computed = 8;      /* terms up to a(8) are computed */
    }
    else if (n > (idx_t)cap) {
        /* grow cache to accommodate requested index */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    if (n <= 8)
        return cache[n - 1];

    /* compute missing terms up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            idx_t idx = i - 1;                    /* zero‑based index in cache */
            seq_t val =
                (seq_t)3 * cache[idx - 1] +
                (seq_t)19 * cache[idx - 2] +
                (seq_t)24 * cache[idx - 3] -
                      cache[idx - 4] -
                (seq_t)22 * cache[idx - 5] -
                (seq_t)11 * cache[idx - 6] -
                (seq_t)2  * cache[idx - 7] -
                (seq_t)2  * cache[idx - 8];
            cache[idx] = val;
        }
        last_computed = n;
    }

    return cache[n - 1];
}

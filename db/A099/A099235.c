/*
 * A099235  A quadrisection of 1/(1-x-x^5).
 * https://oeis.org/A099235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 6*a(n-3) + 4*a(n-4) + a(n-5) Initial
 *            values: a(0)=1, a(1)=1, a(2)=5, a(3)=15, a(4)=45
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t used = 0;  /* number of terms stored */

    size_t k = (size_t)n;    /* index as size_t for array access */

    /* initialise cache on first call */
    if (!cache) {
        /* start with room for a few terms */
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory fallback */

        /* base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)45;
        used = 5;
    }

    /* grow cache if necessary */
    if (k >= used) {
        size_t new_cap = cap;
        while (new_cap <= k) {
            new_cap = new_cap ? new_cap * 2 : 1;
        }
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp) {
                cache = tmp;
                cap = new_cap;
            } else {
                /* allocation failed – return what we have */
                return cache[used - 1];
            }
        }
        /* compute missing terms */
        for (size_t i = used; i <= k; ++i) {
            seq_t val;
            if (i < 5) {
                /* base terms already set */
                val = cache[i];
            } else {
                val = cache[i-1] + (seq_t)4 * cache[i-2] + (seq_t)6 * cache[i-3]
                      + (seq_t)4 * cache[i-4] + cache[i-5];
            }
            cache[i] = val;
        }
        used = k + 1;
    }

    return cache[k];
}

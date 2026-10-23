/*
 * A185743  Number of (n+1)X3 binary arrays with every 2X2 subblock trace
 *          equal to some horizontal or vertical neighbor 2X2 subblock trace.
 * https://oeis.org/A185743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 17*a(n-3) + 2*a(n-4) - 2*a(n-5)
 *            +18*a(n-7) - 4*a(n-8) - 30*a(n-9) - 8*a(n-10) - 24*a(n-11)
 *            Initial values: a(1)=24, a(2)=104, a(3)=544, a(4)=2568,
 *            a(5)=12656, a(6)=62112, a(7)=304584, a(8)=1494072, a(9)=7329384,
 *            a(10)=35956128, a(11)=176387688
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range */

    /* static cache for computed values */
    static seq_t *cache = NULL;      /* array of a(1..cache_size) */
    static idx_t  cache_size = 0;    /* number of terms computed */

    /* initial terms (a(1)..a(11)) */
    static const seq_t init[11] = {
        (seq_t)24,       (seq_t)104,      (seq_t)544,
        (seq_t)2568,     (seq_t)12656,    (seq_t)62112,
        (seq_t)304584,   (seq_t)1494072,  (seq_t)7329384,
        (seq_t)35956128, (seq_t)176387688
    };

    /* on first call, allocate the cache and fill the first 11 terms */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) abort();      /* fatal error */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
    }

    /* returned value is already known */
    if (n <= cache_size) return cache[n - 1];

    /* ensure the cache can hold up to n terms: remember old size */
    idx_t old_size = cache_size;
    if (n > cache_size) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* fatal error */
        cache = tmp;
        cache_size = n;
    }

    /* compute new terms using the empirical recurrence */
    for (idx_t k = old_size + 1; k <= n; ++k) {
        /* k is 1‑based; cache indices are 0‑based: a(k) -> cache[k-1] */

        /* positive contributions */
        seq_t pos = (seq_t)5  * cache[k - 2]          /* a(k‑1) */
                  + (seq_t)17 * cache[k - 4]          /* a(k‑3) */
                  + (seq_t)2  * cache[k - 5]          /* a(k‑4) */
                  + (seq_t)18 * cache[k - 8];        /* a(k‑7) */

        /* negative contributions */
        seq_t sub = (seq_t)4  * cache[k - 3]          /* a(k‑2) */
                  + (seq_t)2  * cache[k - 6]          /* a(k‑5) */
                  + (seq_t)4  * cache[k - 9]          /* a(k‑8) */
                  + (seq_t)30 * cache[k - 10]        /* a(k‑9) */
                  + (seq_t)8  * cache[k - 11]        /* a(k‑10) */
                  + (seq_t)24 * cache[k - 12];       /* a(k‑11) */

        cache[k - 1] = pos - sub;
    }

    return cache[n - 1];
}

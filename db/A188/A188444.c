/*
 * A188444  Expansion of (1+x)*(1+x+x^2)*(1-x+x^2-4*x+x^4-x^5+x^6)/(1+x^4)^3.
 * https://oeis.org/A188444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-4) -3*a(n-8) - a(n-12) for n >= 12 * Initial
 *            values (n < 12): * a(0)=1, a(1)=1, a(2)=1, a(3)=-3, a(4)=-9,
 *            a(5)=-9, * a(6)=-6, a(7)=10, a(8)=25, a(9)=25, a(10)=15,
 *            a(11)=-21
 * Range:     i8 n <= 23, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static table for the first 12 terms */
static const seq_t init12[12] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)1,
    (seq_t)-3,
    (seq_t)-9,
    (seq_t)-9,
    (seq_t)-6,
    (seq_t)10,
    (seq_t)25,
    (seq_t)25,
    (seq_t)15,
    (seq_t)-21
};

static seq_t *cache = NULL;    /* memoisation buffer */
static idx_t   cache_cap = 0;  /* allocated size of cache */
static idx_t   cache_size = 0; /* highest n for which cache[n] is valid */

/* Public sequence routine */
seq_t a(idx_t n) {
    /* Return quickly if value already computed */
    if (n < cache_size)
        return cache[n];

    /* Ensure cache can hold indices up to n */
    idx_t target = n + 1;
    if (cache_cap < target) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < target)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            abort();   /* allocation failure */
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values from the end of the current cache to n */
    for (idx_t i = cache_size; i < target; ++i) {
        if (i < 12) {
            cache[i] = init12[i];
        } else {
            cache[i] = (seq_t)(-3) * cache[i - 4]
                     + (seq_t)(-3) * cache[i - 8]
                     + (seq_t)(-1) * cache[i - 12];
        }
    }
    cache_size = target;
    return cache[n];
}

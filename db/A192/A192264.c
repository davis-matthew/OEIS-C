/*
 * A192264  a(1)=1, a(2)=2; a(n) = abs((n-1)*a(n-1) - n*a(n-2)), n > 2.
 * https://oeis.org/A192264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; a(n) = abs((n-1)*a(n-1) - n*a(n-2)) for n>2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache      = NULL;   /* 0‑based cache storing a(1)..a(n) */
static size_t cache_cap  = 0;      /* allocated capacity in elements    */
static idx_t  cache_max  = 0;      /* largest n already computed         */

static seq_t absdiff(seq_t x, seq_t y) {
    return x > y ? x - y : y - x;
}

seq_t a(idx_t n) {
    /* Undefined offset: return 0 safely */
    if (n == 0) return 0;

    /* Grow cache if needed */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        cache      = tmp;
        cache_cap  = (size_t)n;
    }

    /* Cached value already available */
    if (n <= cache_max) return cache[n - 1];

    /* Compute values sequentially up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 1;
        } else if (k == 2) {
            val = 2;
        } else {
            seq_t t1 = ((seq_t)(k - 1)) * cache[k - 2]; /* (k-1) * a(k-1) */
            seq_t t2 = ((seq_t)k) * cache[k - 3];       /*  k   * a(k-2) */
            val = absdiff(t1, t2);
        }
        cache[k - 1] = val;
    }

    cache_max = n;
    return cache[n - 1];
}

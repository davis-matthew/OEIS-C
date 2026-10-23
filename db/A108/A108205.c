/*
 * A108205  a(n) = 2*(n-1)*a(n-1)+(n-1)*a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A108205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*a(n-1)+(n-1)*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A108205 */
seq_t a(idx_t n)
{
    /*  cache for previously computed values, grows as needed */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;   /* number of values stored (up to cache_size-1) */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* enlarge the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        if (new_cap == 0)
            new_cap = 2;     /* start with a small size */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 (unlikely in test harness) */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)0;
            } else if (i == 1) {
                val = (seq_t)1;
            } else {
                seq_t mul = (seq_t)(i - 1);   /* n-1 converted to seq_t */
                seq_t t = cache[i - 1];
                t += cache[i - 1];            /* 2 * a(i-1) */
                t += cache[i - 2];            /* + a(i-2) */
                val = mul * t;
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}

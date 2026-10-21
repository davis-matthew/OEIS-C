/*
 * A203916  Number of (n+2) X 3 0..1 arrays with every 3 X 3 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..1 introduced in row major order.
 * https://oeis.org/A203916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-3) + 4*a(n-4) Initial values: a(1)=112,
 *            a(2)=392, a(3)=1400, a(4)=5000
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    /* 1‑based cache; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size of cache */
    static size_t max_computed = 0;    /* largest n already computed */

    size_t needed = (size_t)n + 1;       /* indices 0..n are needed */
    if (needed < 5)                      /* ensure indices 1..4 exist */
        needed = 5;

    /* Expand the cache if necessary */
    if (needed > cache_size) {
        size_t new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        /* initialise newly allocated entries to zero */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first four terms once */
    if (cache[1] == (seq_t)0) {
        cache[1] = (seq_t)112;
        cache[2] = (seq_t)392;
        cache[3] = (seq_t)1400;
        cache[4] = (seq_t)5000;
        max_computed = 4;
    }

    if (n <= 4)
        return cache[(size_t)n];

    if (max_computed < 4)
        max_computed = 4;

    /* Compute subsequent terms using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * ((seq_t)3)
                 + cache[i-3] * ((seq_t)6)
                 + cache[i-4] * ((seq_t)4);
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}

/*
 * A062259  Number of (0,1)-strings of length n that avoid the substrings of
 *          substrings 11101011 and 101111.
 * https://oeis.org/A062259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + 2*a(n-7) + 2*a(n-8) +
 *            a(n-9) + a(n-10), with initial values: a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=8, a(4)=16, a(5)=32, a(6)=63, a(7)=124, a(8)=243, a(9)=476.
 *            (OEIS A062259 – number of (0,1)-strings of length n avoiding
 *            11101011 and 101111.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 10 terms */
    static const seq_t base[10] = {
        (seq_t)1,  (seq_t)2,  (seq_t)4,  (seq_t)8,
        (seq_t)16, (seq_t)32, (seq_t)63, (seq_t)124,
        (seq_t)243,(seq_t)476
    };

    /* cache for computed values */
    static seq_t *cache = NULL;
    static idx_t last_computed = 9;   /* highest index already computed */
    static idx_t cache_size  = 0;     /* current allocated size */

    if (n < 10)                /* use table for the first 10 indices */
        return base[n];

    /* allocate / grow cache on first use or when needed */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
    } else if (n >= cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* fill in any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4]
                   + (seq_t)2 * cache[i-7] + (seq_t)2 * cache[i-8]
                   + cache[i-9] + cache[i-10];
    }
    last_computed = n;

    return cache[n];
}

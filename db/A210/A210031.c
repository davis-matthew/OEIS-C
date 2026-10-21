/*
 * A210031  Number of binary words of length n containing no subword 100001.
 * https://oeis.org/A210031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n < 6 a(n) = 2*a(n-1) - a(n-5) + a(n-6) for n >=
 *            6 See A210031: Number of binary words of length n containing no
 *            subword 100001
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /*  Static cache to memoise previously computed values. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of entries stored */
    static idx_t  cache_cap = 0;   /* current capacity of cache array */

    if (n < cache_len)
        return cache[n];          /* already computed */

    /* Ensure enough capacity to hold up to n. */
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;            /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {                    /* allocation failure – unlikely */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;

    /* Compute missing terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6) {
            /* 2^i for i = 0..5, safe for any seq_t (shifts less than 64). */
            cache[i] = ((seq_t)1) << i;
        } else {
            seq_t val = cache[i-1];
            val = val * (seq_t)2;   /* 2 * a(i-1) */
            val = val - cache[i-5]; /* subtract a(i-5) */
            val = val + cache[i-6]; /* add a(i-6) */
            cache[i] = val;
        }
    }
    cache_len = n + 1;
    return cache[n];
}

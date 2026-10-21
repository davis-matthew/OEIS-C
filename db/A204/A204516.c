/*
 * A204516  Numbers such that floor(a(n)^2 / 7) is a square.
 * https://oeis.org/A204516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-3) - a(n-6) for n >= 8, * with a(1)=0, a(2)=1,
 *            a(3)=2, a(4)=3, a(5)=8, a(6)=16, a(7)=45
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 51, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache with the first 7 terms */
    if (cache == NULL) {
        cache_cap = 16; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();               /* allocation failure */
        cache[0] = (seq_t)0;  /* a(1) */
        cache[1] = (seq_t)1;  /* a(2) */
        cache[2] = (seq_t)2;  /* a(3) */
        cache[3] = (seq_t)3;  /* a(4) */
        cache[4] = (seq_t)8;  /* a(5) */
        cache[5] = (seq_t)16; /* a(6) */
        cache[6] = (seq_t)45; /* a(7) */
        cache_len = 7;
    }

    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n > new_cap) {
            new_cap *= 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms using recurrence a(k) = 16*a(k-3) - a(k-6) */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] = ((seq_t)16 * cache[i - 3]) - cache[i - 6];
    }
    cache_len = n;

    return cache[n - 1];
}

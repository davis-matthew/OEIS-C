/*
 * A192928  The Gi1 and Gi2 sums of Losanitsch's triangle A034851.
 * https://oeis.org/A192928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-6) + a(n-8) -
 *            a(n-9) - a(n-12) (Recurrence derived from the rational
 *            generating function)
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values stored in cache */
    static idx_t cache_capacity = 0;

    /* Base values for n = 0 .. 11 */
    static const seq_t base[12] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)3,
        (seq_t)5, (seq_t)6, (seq_t)9, (seq_t)11
    };

    if (n < 12) {
        return base[n];
    }

    idx_t needed = n + 1;
    if (needed > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    if (cache_size == 0) {
        /* initialise cache with the first 12 terms */
        for (idx_t i = 0; i < 12; ++i) cache[i] = base[i];
        cache_size = 12;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 0;
        val += cache[i-1];
        val += cache[i-2];
        val -= cache[i-3];
        val += cache[i-4];
        /* i-5 unused */
        val -= cache[i-6];
        /* i-7 unused */
        val += cache[i-8];
        val -= cache[i-9];
        /* i-10, i-11 unused */
        val -= cache[i-12];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}

/*
 * A207495  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 14*a(n-3) + 6*a(n-4) + 8*a(n-5) -
 *            a(n-6) - a(n-7); a(1)=6, a(2)=36, a(3)=102, a(4)=297, a(5)=932,
 *            a(6)=2974, a(7)=9723, a(8)=32164
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;    /* number of terms cached,
                                     cache[0] = a(1), etc. */

    /* first call: initialise with the first 8 terms and allocate cache */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        static const seq_t init[8] = {6, 36, 102, 297, 932, 2974, 9723, 32164};
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        cache_len = 8;
    }
    /* grow or shrink cache as required for the requested index */
    else if ((idx_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if the value was already computed, return it immediately */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t next = 5 * cache[i - 2]
                     - 2 * cache[i - 3]
                     - 14 * cache[i - 4]
                     + 6 * cache[i - 5]
                     + 8 * cache[i - 6]
                     - cache[i - 7]
                     - cache[i - 8];
        cache[i - 1] = next;
    }
    cache_len = n;
    return cache[n - 1];
}

/*
 * A208674  Number of words, either empty or beginning with the first letter
 *          of the n-ary alphabet, where each letter of the alphabet occurs 3
 *          times and letters of neighboring word positions are equal or
 *          neighbors in the alphabet.
 * https://oeis.org/A208674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=1,a(2)=10,a(3)=37,a(4)=163,
   a(n)=5*a(n-1)-a(n-2)-14*a(n-3)-2*a(n-4)+4*a(n-5) (n≥5) */

#include <stdlib.h>

static const seq_t base_vals[5] = {1, 1, 10, 37, 163};

seq_t a(idx_t n)
{
    /* Return small values directly. */
    if (n < 5) {
        return base_vals[n];
    }

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (number of seq_t slots) */
    static size_t cache_len = 0;   /* number of terms already computed */

    /* Grow allocation if necessary. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < (size_t)(n + 1)) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* out of memory – terminate */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache with the first five base values if empty. */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 5; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = 5;
    }

    /* Compute terms up to n lazily. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t tmp = 5 * cache[i - 1];
        tmp -= cache[i - 2];
        tmp -= 14 * cache[i - 3];
        tmp -= 2 * cache[i - 4];
        tmp += 4 * cache[i - 5];
        cache[i] = tmp;
    }
    cache_len = (size_t)(n + 1);

    return cache[n];
}

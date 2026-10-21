/*
 * A209084  a(n) = 2*a(n-1) + 4*a(n-2) with n>1, a(0)=0, a(1)=4.
 * https://oeis.org/A209084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) with a(0)=0, a(1)=4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = (seq_t *)NULL;
    static size_t cache_len = 0;  /* number of terms currently stored */
    static size_t cache_cap = 0;  /* allocation capacity in terms */

    /* Initialisation: store the two seed values. */
    if (cache == (seq_t *)NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == (seq_t *)NULL) { return (seq_t)0; }  /* allocation fail */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Need to store up to index n inclusive. */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            /* grow capacity to twice the needed size for future calls */
            size_t new_cap = needed * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (tmp == (seq_t *)NULL) { /* realloc failed – keep old cache */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n. */
        while (cache_len <= (size_t)n) {
            seq_t prev2 = cache[cache_len - 2];
            seq_t prev1 = cache[cache_len - 1];
            cache[cache_len] = prev1 * (seq_t)2 + prev2 * (seq_t)4;
            ++cache_len;
        }
    }

    return cache[(size_t)n];
}

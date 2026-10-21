/*
 * A068912  Number of n step walks (each step +/-1 starting from 0) which are
 *          never more than 3 or less than -3.
 * https://oeis.org/A068912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8; for n>=4: a(n) = 4*a(n-2) -
 *            2*a(n-4) (A068912)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(size_t *cache_len, size_t *cache_cap, seq_t **cache)
{
    if (*cache_len >= *cache_cap) {
        *cache_cap <<= 1;
        *cache = (seq_t *)realloc(*cache, (*cache_cap) * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialisation of the table on the first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache_len = 4;
    }

    /* If the requested index is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute successive terms up to n */
    while (cache_len <= n) {
        ensure_capacity(&cache_len, &cache_cap, &cache);

        if (cache_len >= 4) {
            /* a(n) = 4*a(n-2) - 2*a(n-4) */
            seq_t val = ((seq_t)4) * cache[cache_len - 2]
                      - ((seq_t)2) * cache[cache_len - 4];
            cache[cache_len] = val;
        }
        /* For indices 0-3, values are already set. */
        cache_len++;
    }

    return cache[n];
}

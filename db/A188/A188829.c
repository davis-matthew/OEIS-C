/*
 * A188829  Number of 7 X n binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 25*a(n-3) + 25*a(n-4) with a(1)=128, a(2)=64,
 *            a(3)=224, a(4)=784, a(5)=2800
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values, 1‑based indexing */
    static size_t cache_cap = 0;    /* allocated array size (in slots) */
    static idx_t  cache_max = 0;    /* largest index already computed */

    /* base cases – do the one‑time allocation and initialization */
    if (!cache) {
        cache_cap = 8;                      /* a bit larger than needed */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed – unlikely */

        cache[1] = 128;
        cache[2] = 64;
        cache[3] = 224;
        cache[4] = 784;
        cache[5] = 2800;
        cache_max = 5;
    }

    /* if n is already cached, just return it */
    if (n <= cache_max)
        return cache[n];

    /* ensure we have enough space for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* memory failure – abort gracefully */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute successive terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* recurrence: a(i) = 5*a(i-1) - 25*a(i-3) + 25*a(i-4) */
        seq_t val = 5 * cache[i - 1];
        val -= 25 * cache[i - 3];
        val += 25 * cache[i - 4];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}

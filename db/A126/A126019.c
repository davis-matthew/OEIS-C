/*
 * A126019  a(0)=1, a(1)=2; for n>1, a(n)=3*a(n-1)+4*a(n-2)+5.
 * https://oeis.org/A126019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; for n>1, a(n)=3*a(n‑1)+4*a(n‑2)+5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoisation cache */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;

/* Return the nth term of A126019. */
seq_t a(idx_t n)
{
    /* Initialise cache once. */
    if (cache_size == 0) {
        cache_cap = 2;                                 /* at least 0,1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                    /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0 || i == 1) continue;               /* already initialised */
        cache[i] = (seq_t)3 * cache[i-1] + (seq_t)4 * cache[i-2] + (seq_t)5;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}

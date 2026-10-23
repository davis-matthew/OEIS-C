/*
 * A015534  Expansion of x/(1 - 4*x - 11*x^2).
 * https://oeis.org/A015534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 11*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of OEIS A015534. */
seq_t a(idx_t n)
{
    /* Handle the initial two terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache to store previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of entries stored */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* Allocate initial cache on first call. */
    if (cache == NULL) {
        cache_cap = 4;                       /* arbitrary small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Grow the cache until it contains a(n). */
    while (cache_len <= n) {
        idx_t i = cache_len;                 /* compute a[i] */

        /* Ensure we have space for the new element. */
        if (cache_len >= cache_cap) {
            idx_t newcap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* out‑of‑memory: return 0 */
            cache = tmp;
            cache_cap = newcap;
        }

        /* a[i] = 4*a[i-1] + 11*a[i-2] */
        cache[cache_len] = cache[i-1] * (seq_t)4 + cache[i-2] * (seq_t)11;
        cache_len++;
    }

    return cache[n];
}

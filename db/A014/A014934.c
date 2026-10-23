/*
 * A014934  a(1)=1, a(n) = n*17^(n-1) + a(n-1).
 * https://oeis.org/A014934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = n*17^(n-1) + a(n-1), n ≥ 2 (equivalently a(n) =
 *            ((16*n-1)*17^n + 1)/256)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* a[1] .. a[max] */
    static idx_t cached_up_to = 0;       /* highest index computed */
    static seq_t pow_at_index = 0;       /* 17^(cached_up_to) */

    /* initialise on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));  /* indices 0 and 1 */
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[1] = (seq_t)1;                        /* a(1) */
        cached_up_to = 1;
        pow_at_index = (seq_t)17;                   /* 17^1 for next term */
    }

    if (n <= cached_up_to) { /* already computed */
        return cache[n];
    }

    /* grow the cache to accommodate n */
    idx_t new_size = n + 1; /* indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0; /* allocation failure */
    cache = new_cache;

    /* compute new terms */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * pow_at_index;      /* i*17^(i-1) */
        cache[i] = cache[i - 1] + term;            /* a(i) = a(i-1)+term */
        pow_at_index *= (seq_t)17;                 /* advance to 17^i */
    }

    cached_up_to = n;
    return cache[n];
}

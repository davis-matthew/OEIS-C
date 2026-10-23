/*
 * A033280  Number of diagonal dissections of a convex (n+8)-gon into n+1
 *          regions.
 * https://oeis.org/A033280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5, 5)*binomial(2*n+7, n)/(n+1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache for previously computed values */
static seq_t *seq_cache = NULL;
static idx_t  cache_len  = 0;  /* number of entries currently stored */
static idx_t  cache_cap  = 0;  /* allocated size of seq_cache */

/* Return a(n) for any non‑negative n */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) = 1 if empty */
    if (cache_cap == 0) {
        cache_cap = 16;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
        seq_cache[0] = (seq_t)1;
        cache_len  = 1;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {             /* a(0) already set */
            seq_cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev = seq_cache[i-1];

        /* compute the rational factor:
           num = 2*(i+5)*(i+3)*(2*i+7)
           den = i*(i+7)*(i+1)              */
        unsigned __int128 num = (unsigned __int128)2
                               * (i + 5) * (i + 3) * (2 * i + 7);
        unsigned __int128 den = (unsigned __int128)i * (i + 7) * (i + 1);

        /* multiply prev by num, divide by den, safely in 128‑bit */
        unsigned __int128 prod = (unsigned __int128)prev * num;
        seq_t val = (seq_t)(prod / den);

        seq_cache[i] = val;
    }

    cache_len = n + 1;
    return seq_cache[n];
}

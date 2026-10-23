/*
 * A028009  Expansion of 1/((1-2x)(1-7x)(1-9x)(1-10x)).
 * https://oeis.org/A028009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (35*10^(n+3)-60*9^(n+3)+4*7^(n+4)-3*2^(n+3))/840
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static size_t cache_cap  = 0;

    /* Initialise the cache with the first four values. */
    if (cache_len == 0) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache_cap = init_cap;
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)28;         /* a(1) */
        cache[2] = (seq_t)509;        /* a(2) */
        cache[3] = (seq_t)7628;       /* a(3) */
        cache_len = 4;
    }

    /* Grow the cache until it contains a(n). */
    while (cache_len <= n) {
        /* Resize if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence:
           a(k) = 28*a(k-1) - 275*a(k-2) + 1076*a(k-3) - 1260*a(k-4) */
        seq_t val = (seq_t)28   * cache[cache_len - 1];
        val -= (seq_t)275 * cache[cache_len - 2];
        val += (seq_t)1076 * cache[cache_len - 3];
        val -= (seq_t)1260 * cache[cache_len - 4];

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}

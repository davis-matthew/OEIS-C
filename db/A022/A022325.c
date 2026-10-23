/*
 * A022325  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 11.
 * https://oeis.org/A022325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 with a(0) = 1, a(1) = 11
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = (seq_t*)NULL;
static size_t cache_cap  = 0;
static idx_t   cache_len  = 0;

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (cache_len == 0) {
        /* start with a small capacity, will grow as needed */
        cache_cap = 8;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);          /* allocation failed */
        seq_cache[0] = 1;
        seq_cache[1] = 11;
        cache_len   = 2;
    }

    /* grow cache if n is beyond current capacity */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(seq_cache,
                                    new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms once and store them */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_cache[i] = seq_cache[i-1] + seq_cache[i-2] + (seq_t)1;
        }
        cache_len = n + 1;
    }

    return seq_cache[n];
}

/*
 * A115833  Integers i such that 16*i XOR 17*i = 33*i.
 * https://oeis.org/A115833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 36, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n‑th integer i such that 16*i XOR 17*i = 33*i */

#include <stdlib.h>
#include <stdbool.h>

/* Check the defining property for a candidate value. */
static bool is_good(seq_t val)
{
    seq_t x = val << 4;                /* 16*i */
    return (x & (x + val)) == 0;       /* 16*i & (16*i + i) == 0 */
}

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 0;      /* next number to test */

    /* Generate terms until we have at least n+1 entries */
    while (cache_len <= (size_t)n) {
        /* Find the next integer that satisfies the condition */
        while (!is_good(next_candidate)) {
            next_candidate++;
        }

        /* Grow the cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next_candidate;
        next_candidate++;                 /* move past the accepted value */
    }

    return cache[(size_t)n];
}

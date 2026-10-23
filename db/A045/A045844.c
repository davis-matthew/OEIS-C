/*
 * A045844  a(n+1) = a(n) + largest digit of a(n); a(0) = 1.
 * https://oeis.org/A045844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n+1) = a(n) + largest digit of a(n); a(0) = 1
 * Range:     u8 n <= 47, u16 n <= 9333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_digit_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/max_digit_seq_t.h"

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache starts with a(0) = 1 */
    if (cache == NULL)
    {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)
            return 0;          /* allocation failure – return 0 as sentinel */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute up to n if not already done */
    while (cache_len <= n)
    {
        /* Grow the cache if needed */
        if (cache_len == cache_size)
        {
            size_t new_size = cache_size * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp)
                return 0;      /* allocation failure – return 0 as sentinel */
            cache = tmp;
            cache_size = new_size;
        }

        /* Use the recurrence to compute the next term */
        seq_t prev = cache[cache_len - 1];
        seq_t add  = max_digit_seq_t(prev);
        cache[cache_len] = prev + add;
        cache_len++;
    }

    return cache[n];
}

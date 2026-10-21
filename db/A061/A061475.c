/*
 * A061475  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 6.
 * https://oeis.org/A061475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer having alternating digit sum 6
 *            (first digit − second digit + third digit − … = 6)
 * Range:     u8 n <= 15, u16 n <= 3758, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alt_digit_sum.h"

/* Cache storage for already-found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;

/* Add a new term to the cache, enlarging the array if required. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of sequence A061475 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    while (cache_len < n) {
        if (alt_digit_sum(next_candidate) == 6)
            cache_push(next_candidate);
        ++next_candidate;
    }
    return cache[n - 1];
}

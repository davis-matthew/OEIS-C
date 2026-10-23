/*
 * A031987  Numbers with exactly five distinct base-10 digits.
 * https://oeis.org/A031987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with exactly five distinct decimal digits, sorted ascending. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static bool has_five_distinct_digits(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    int distinct = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i])
            ++distinct;
    return distinct == 5;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but safe */

    size_t len = (size_t)n;

    /* Ensure the cache has enough capacity. */
    if (cache_cap < len) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < len)
            new_cap = len;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return a cached value if available. */
    if (len <= cache_len)
        return cache[len - 1];

    /* Start searching from the first missing term. */
    seq_t num = (cache_len == 0) ? (seq_t)10234 : cache[cache_len - 1] + (seq_t)1;

    for (size_t i = cache_len; i < len; ++i) {
        while (!has_five_distinct_digits(num))
            ++num;
        cache[i] = num;
        ++num;   /* advance to next candidate for the following term */
    }

    cache_len = len;
    return cache[len - 1];
}

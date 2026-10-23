/*
 * A166459  Numbers whose sum of digits is 19.
 * https://oeis.org/A166459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: computes the nth positive integer whose decimal digit sum is 19 by incremental search */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = (seq_t)199;

static void grow_cache(void)
{
    size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr)
        abort();
    cache      = newptr;
    cache_cap  = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    size_t target = (size_t)(n - 1);

    if (target < cache_len)
        return cache[target];

    while (cache_len <= target) {
        seq_t val = next_candidate;
        ++next_candidate;

        if (digit_sum(val) == (seq_t)19) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = val;
        }
    }
    return cache[target];
}

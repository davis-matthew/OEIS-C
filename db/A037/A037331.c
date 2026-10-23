/*
 * A037331  Numbers whose base-7 and base-8 expansions have the same digit
 *          sum.
 * https://oeis.org/A037331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a search for numbers whose base‑7 and base‑8 digit sums are equal. */

#include <stddef.h>
#include "../utils/seq_alloc.h"

static seq_t *cache          = NULL; /* array of found numbers               */
static size_t cache_cap      = 0;    /* capacity of cache                    */
static size_t cache_used     = 0;    /* number of valid entries in cache     */
static seq_t last_checked    = 0;    /* last number tested                   */

/* Ensure the cache has room for at least 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = seq_alloc(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_used; ++i)
            new_cache[i] = cache[i];
        /* previous cache is discarded; no explicit free is needed. */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Digit sum of x in the given base (base is 7 or 8). */
static seq_t digit_sum_base(seq_t x, seq_t base)
{
    seq_t sum = 0;
    while (x) {
        sum += x % base;
        x /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0; /* the sequence is undefined for n < 1 */

    /* Expand cache until it contains at least n entries. */
    while (cache_used < (size_t)n) {
        seq_t candidate = last_checked + (seq_t)1;
        if (digit_sum_base(candidate, (seq_t)7) == digit_sum_base(candidate, (seq_t)8)) {
            grow_cache(cache_used + 1);
            cache[cache_used++] = candidate;
        }
        last_checked = candidate;
    }
    return cache[(size_t)n - 1];
}

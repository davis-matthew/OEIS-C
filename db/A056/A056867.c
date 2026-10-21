/*
 * A056867  Nilpotent numbers: n such that every group of order n is
 *          nilpotent.
 * https://oeis.org/A056867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_nilpotent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nilpotent numbers: n is nilpotent iff for every pair of distinct primes
   p and q dividing n, we have p^k ≢ 1 (mod q) for all 1 ≤ k ≤ exp_p(n). */

/* The code searches for nilpotent numbers, caching results so that
   any index can be answered in any order. No external helpers are used. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_nilpotent.h"

/* Static cache of found nilpotent numbers. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t counter = 0;      /* integer currently being tested */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* out of memory */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  Returns the n-th nilpotent number (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Guard against zero index – undefined, but return 0. */
    if (n == 0)
        return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        ++counter;
        if (is_nilpotent(counter))
        {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)counter;
        }
    }
    return cache[n - 1];
}

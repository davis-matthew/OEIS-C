/*
 * A102114  Iccanobirt numbers (4 of 15): a(n) = R(a(n-1)) + a(n-2) + a(n-3),
 *          where R is the digit reversal function A004086.
 * https://oeis.org/A102114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = R(a(n-1)) + a(n-2) + a(n-3) with a(0)=0, a(1)=0, a(2)=1,
 *            R = digit reversal (A004086)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 65, u128 n <= 123,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rev.h"

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_len = 0;   /* number of cached terms (non‑negative) */
static size_t cache_cap = 0;  /* allocated capacity for cache */

/* Ensure the cache can hold at least `needed` elements.  */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)needed)
        new_cap *= 2;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    grow_cache(n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 0;
        else if (i == 2)
            cache[i] = 1;
        else {
            seq_t rev_prev = rev(cache[i - 1]);   /* digit reversal of a(i-1) */
            cache[i] = rev_prev + cache[i - 2] + cache[i - 3];
        }
    }

    cache_len = n + 1;
    return cache[n];
}

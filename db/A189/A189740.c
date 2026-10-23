/*
 * A189740  Partial sums of tetranacci numbers (A000288).
 * https://oeis.org/A189740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5), a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=8
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Ensure cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    ensure_cache(n);

    /* initialise base cases if not already */
    if (cache_max < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)8;
        cache_max = 5;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)(2 * (seq_t)cache[i - 1] - (seq_t)cache[i - 5]);
    }
    cache_max = (n > cache_max) ? n : cache_max;

    return cache[n];
}

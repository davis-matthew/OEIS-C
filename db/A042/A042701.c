/*
 * A042701  Denominators of continued fraction convergents to sqrt(880).
 * https://oeis.org/A042701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 178*a(n-4) - a(n-8) for n>7 Initial terms: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=176, a(5)=179, a(6)=355, a(7)=534
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first 8 terms if not already done. */
static void init_cache(void)
{
    if (cache_len >= 8)
        return;

    ensure_cache_capacity(7);        /* indices 0 .. 7 */
    cache[0] = 1;
    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 3;
    cache[4] = 176;
    cache[5] = 179;
    cache[6] = 355;
    cache[7] = 534;
    cache_len = 8;
}

seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n >= cache_len) {
        ensure_cache_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = cache[i-4] * (seq_t)178;
            term -= cache[i-8];
            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}

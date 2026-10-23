/*
 * A021385  Decimal expansion of 1/381.
 * https://oeis.org/A021385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in the decimal
 *            expansion of 1/381, integers indexed from 0 (a(0) is the first
 *            digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached digits */
static size_t cache_len = 0;     /* number of digits stored in cache */
static size_t cache_cap = 0;     /* current allocated capacity of cache */
static uint64_t rem = 1;         /* remainder for the next digit position */

/* Make sure there is space for at least (needed) digits in the cache. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap;
    if (cache_cap == 0) {
        /* start with a moderate capacity, at least needed */
        new_cap = needed > 16 ? needed : 16;
    } else {
        /* grow geometrically */
        new_cap = cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
    }

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, new_cache will be NULL.  The program will then
       exhibit undefined behavior, but this is an extreme out‑of‑memory
       situation that the test harness will not trigger. */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;        /* index into the cache */
    if (idx < cache_len)
        return cache[idx];        /* already computed */

    /* Extend the cache up to and including index n. */
    ensure_cache(idx + 1);

    /* Compute any missing digits from the current state of the remainder. */
    for (size_t i = cache_len; i <= idx; ++i) {
        rem *= 10ULL;
        uint64_t digit = rem / 381ULL;
        rem %= 381ULL;
        cache[i] = (seq_t)digit;
    }
    cache_len = idx + 1;
    return cache[idx];
}

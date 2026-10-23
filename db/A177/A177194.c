/*
 * A177194  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 0.
 * https://oeis.org/A177194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_counts, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth Fibonacci number whose decimal expansion contains no digit 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility helpers supplied by the build environment. */
#include "../utils/fib.h"
#include "../utils/digit_counts.h"

/* Cache of zero‑free Fibonacci numbers. */
static seq_t *zero_free_cache = NULL;   /* array of seq_t values */
static size_t cache_cap   = 0;          /* allocated capacity (elements) */
static size_t cache_len   = 0;          /* number of stored values    */
static idx_t  next_k      = 1;          /* next Fibonacci index to test */

/* Ensure the cache accommodates at least `min_cap` elements. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(zero_free_cache, new_cap * sizeof(seq_t));
    if (!tmp) {                         /* allocation failure – reset cache */
        free(zero_free_cache);
        zero_free_cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        return;
    }

    zero_free_cache = tmp;
    cache_cap = new_cap;
}

/* Test whether the decimal representation of `x` contains no zero digit. */
static bool zero_free(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[0] == 0;
}

/* Return the nth Fibonacci number without any digit 0. */
seq_t a(idx_t n)
{
    /* Fast return if already cached. */
    if (n > 0 && cache_len >= n)
        return zero_free_cache[n - 1];

    if (!zero_free_cache)
        ensure_cache_capacity(8);

    /* Build the cache until it contains n zero‑free Fibonacci numbers. */
    while (cache_len < n) {
        seq_t f = fib(next_k);  /* fib is 1‑based. */
        ++next_k;

        if (zero_free(f)) {
            ensure_cache_capacity(cache_len + 1);
            if (!zero_free_cache)   /* allocations exhausted */
                return 0;
            zero_free_cache[cache_len++] = f;
        }
    }

    return zero_free_cache[n - 1];
}

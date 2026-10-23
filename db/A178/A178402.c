/*
 * A178402  Numbers not containing the rounded up arithmetic mean of their
 *          digits, cf. A004427.
 * https://oeis.org/A178402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer whose decimal digits do not
 *            contain the rounded-up arithmetic mean of its digits. Offset = 1
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ceil_mean
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ceil_mean.h"

/* memoisation tables */
static seq_t *cache = NULL;          /* stored terms */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* capacity of cache array */
static seq_t next_candidate = 1;     /* integer to test next */

/* ensure cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);          /* fatal allocation error */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* true iff digit d (0..9) occurs in x */
static bool has_digit(seq_t x, int d)
{
    while (x > 0) {
        int digit = (int)(x % 10);
        if (digit == d) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; keep behaviour benign */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        int m = ceil_mean(next_candidate);
        bool contains = has_digit(next_candidate, m);
        if (!contains) {
            ensure_capacity(cache_len + 1);
            cache[cache_len] = next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

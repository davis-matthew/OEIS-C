/*
 * A212499  Numbers k that divide the product of digits of k.
 * https://oeis.org/A212499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer that contains a digit 0 or is <= 9
 * (i.e. every k such that k divides the product of its decimal digits).
 * This sequence consists of 1…9 followed by all numbers with at least one zero digit.
 * The function maintains a cache and extends it on demand. */

#include <stdlib.h>
#include <stdbool.h>

static bool has_zero_digit(uint64_t x) {
    while (x > 0) {
        if ((x % 10) == 0u)
            return true;
        x /= 10u;
    }
    return false;
}

static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of cached elements */
static size_t cache_cap  = 0;   /* allocated capacity */
static uint64_t next_candidate = 1u;

static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a production setting we would handle allocation failure.
           For this benchmark environment we simply exit. */
        abort();
    }
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* n is 1‑based; no negative indices are used */
    if (n == 0u)
        return (seq_t)0;  /* Undefined for offset 1, but guard anyway */

    /* Extend cache until it contains the n-th term */
    while (cache_len < n) {
        /* Check next_candidate */
        if (next_candidate <= 9u || has_zero_digit(next_candidate)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1u];
}

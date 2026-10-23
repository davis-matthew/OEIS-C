/*
 * A127271  Numbers n such that sum of the digits of n and n+2 divides n+1, n
 *          >= 1.
 * https://oeis.org/A127271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127271: numbers n such that sumdigits(n)+sumdigits(n+2) divides n+1. */
/* Computation is by brute‑force generation with memoisation. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Small dynamic array to cache the sequence values. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of terms stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety; only n>=1 is used */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Ensure the cache can hold at least n terms. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have the n‑th one. */
    while (cache_len < (size_t)n) {
        seq_t start = (cache_len > 0) ? cache[cache_len - 1] + 1 : 1;
        for (seq_t x = start; cache_len < (size_t)n; ++x) {
            seq_t sum = digit_sum(x) + digit_sum(x + 2);
            if (sum != 0 && ((x + 1) % sum == 0)) {
                cache[cache_len++] = x;
            }
        }
    }

    return cache[n - 1];
}

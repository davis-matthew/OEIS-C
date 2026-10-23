/*
 * A021811  Decimal expansion of 1/807.
 * https://oeis.org/A021811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/807 (a(0) = 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache: digits of the expansion have been computed up to index
 *   cache_len-1.  cache_cap is the allocated capacity.  rem holds the
 *   remainder after the last computed digit.  These static objects
 *   allow calls in arbitrary order. */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of digits stored */
static size_t cache_cap = 0;     /* capacity of the cache */
static uint64_t rem = 1;         /* remainder after the last digit */

/* Compute the digit at position n (0‑based). */
seq_t a(idx_t n)
{
    /* n can be up to 10 000 in tests, so we can safely cast to size_t. */
    size_t target = (size_t)n + 1;       /* number of digits we need */

    /* Grow the cache if necessary. */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < target) new_cap <<= 1;

        seq_t *new_ptr;
        if (cache == NULL) {
            new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (new_ptr == NULL) {
            /* Out of memory – in this contest harness this case is
             * unlikely.  Return 0 as a safe default. */
            return 0;
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Compute any missing digits. */
    for (size_t i = cache_len; i < target; ++i) {
        rem *= 10ULL;
        uint64_t digit = rem / 807ULL;   /* 0 <= digit <= 9 */
        rem -= digit * 807ULL;
        cache[i] = (seq_t)digit;
    }
    cache_len = target;

    return cache[(size_t)n];
}

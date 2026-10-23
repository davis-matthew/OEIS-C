/*
 * A021877  Decimal expansion of 1/873.
 * https://oeis.org/A021877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth digit after the decimal point in the expansion of 1/873 */
#include <stdlib.h>
#include <stdint.h>

/* Cache state */
static seq_t *cache = NULL;   /* digit array */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_next = 0;  /* index of first uncached digit */
static uint64_t rem = 1;       /* remainder for the long‑division algorithm */

seq_t a(idx_t n)
{
    /* If we already have the digit, return it */
    if ((size_t)n < cache_next)
        return cache[(size_t)n];

    /* Ensure we have enough capacity */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute digits from cache_next up to n */
    for (size_t i = cache_next; i <= (size_t)n; ++i) {
        rem *= 10;
        seq_t digit = (seq_t)(rem / 873);
        cache[i] = digit;
        rem %= 873;
    }
    cache_next = (size_t)n + 1;

    return cache[(size_t)n];
}

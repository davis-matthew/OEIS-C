/*
 * A021699  Decimal expansion of 1/695.
 * https://oeis.org/A021699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit in the decimal expansion of 1/695
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache for the digits 0‑9. */
static seq_t *digit_cache = NULL;
static size_t cache_len = 0;      /* digits that have been computed */
static size_t cache_cap = 0;      /* allocated capacity of digit_cache */
static uint64_t remainder = 1;    /* remainder before the next digit */
static bool cache_inited = false;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
    {
        /* memory allocation failed – terminate the program */
        abort();
    }
    digit_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!cache_inited)
    {
        /* initialise the cache */
        digit_cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        remainder = 1;
        cache_inited = true;
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_len)
    {
        ensure_cache_cap((size_t)n + 1);

        for (size_t i = cache_len; i <= (size_t)n; ++i)
        {
            /* perform one step of long division: 1/695 */
            remainder *= 10ULL;
            uint64_t digit = remainder / 695ULL;
            remainder %= 695ULL;
            digit_cache[i] = (seq_t)digit;
        }
        cache_len = (size_t)n + 1;
    }

    return digit_cache[(size_t)n];
}

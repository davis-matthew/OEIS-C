/*
 * A021955  Decimal expansion of 1/951.
 * https://oeis.org/A021955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 951) mod 10 (decimal digit of 1/951)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached digits */
static size_t  cache_cap = 0;        /* current capacity of cache */
static idx_t   next_needed = 0;      /* number of digits stored so far */
static uint64_t rem = 1;             /* remainder for long division (1 mod 951) */

seq_t a(idx_t n)
{
    /* If we already computed the digit, return it immediately. */
    if (n < next_needed) return cache[n];

    /* Grow the cache as needed. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
    }

    /* Generate digits up to index n. */
    while (next_needed <= n) {
        /* Compute the next decimal digit by long division. */
        uint64_t digit = (rem * 10) / 951;
        rem          = (rem * 10) % 951;

        /* Make sure the cache can hold the new digit. */
        if (next_needed >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (next_needed >= new_cap) new_cap = next_needed + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[next_needed] = (seq_t)digit;
        ++next_needed;
    }

    return cache[n];
}

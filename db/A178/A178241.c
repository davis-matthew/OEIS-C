/*
 * A178241  Numbers n such that d(1)^p + d(2)^(p-1) +... + d(p)^1 is a square,
 *          where d(i), i=1..p, are the digits of n.
 * https://oeis.org/A178241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt, pow_uint_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178241: a(n) = the n‑th integer whose digits
   d(1)^p + d(2)^(p-1)+…+d(p)^1 is a perfect square.
   The function builds the list lazily and caches it. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t) */
#include "../utils/pow_uint_simple.h"

static bool is_term(seq_t x)
{
    /* Gather digits least‑to‑most into a small buffer. */
    uint8_t digits[32];
    size_t len = 0;
    seq_t tmp = x;
    if (tmp == 0) /* zero has a single digit 0 */
        len = 1;
    else
        while (tmp > 0)
        {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }

    /* Compute weighted digit sum with increasing exponents. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += pow_uint_simple(digits[i], (unsigned)(i + 1));

    seq_t r = floor_sqrt(sum);
    return r * r == sum;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache_len == 0) /* initialise with a(0) = 0 */
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache_len = 1;
    }

    while (cache_len <= (size_t)n) /* extend until n is present */
    {
        seq_t candidate = cache[cache_len - 1] + 1;
        while (!is_term(candidate))
            ++candidate;

        if (cache_len >= cache_cap)   /* grow the cache if needed */
        {
            cache_cap = cache_cap ? cache_cap * 2 : 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = candidate;
    }

    return cache[(size_t)n];
}

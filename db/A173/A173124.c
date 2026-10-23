/*
 * A173124  a(n) = binomial(n+10,10)*6^n.
 * https://oeis.org/A173124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10) * 6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 16, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    if (cache_len == 0) {            /* initialise a(0) */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* a(k) = a(k-1) * 6 * (k+10) / k */
        seq_t prev = cache[k - 1];
        seq_t num1 = prev;                 /* a(k-1)   */
        seq_t num2 = (seq_t)6;             /* 6        */
        seq_t num3 = (seq_t)(k + 10);      /* k+10     */
        seq_t denom = (seq_t)k;            /* k        */

        /* Reduce each numerator with the denominator to avoid overflow */
        seq_t g;
        g = gcd(num1, denom);
        if (g > 1) {
            num1 /= g;
            denom /= g;
        }
        g = gcd(num2, denom);
        if (g > 1) {
            num2 /= g;
            denom /= g;
        }
        g = gcd(num3, denom);
        if (g > 1) {
            num3 /= g;
            denom /= g;
        }

        /* After reductions the denominator must be 1 because the
           recurrence always yields an integer. */
        if (denom != (seq_t)1)
            denom = (seq_t)1; /* safety, should never happen */

        /* Multiply the remaining numerators */
        seq_t val = num1;
        val *= num2;
        val *= num3;
        cache[k] = val;
    }

    cache_len = (size_t)n + 1;
}

/* Seq function definition */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

/*
 * A032779  Integer quotients n(n+1)(n+2)...(n+7) / (n+(n+1)+(n+2)+...+(n+7)).
 * https://oeis.org/A032779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Integer quotients * a(n) = [ n(n+1)(n+2)…(n+7) ] / [
 *            n+(n+1)+…+(n+7) ] * offset: 1 (a(1) corresponds to n = 0) * Only
 *            the terms where the division is exact are kept
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* cache of computed terms               */
static size_t cache_size = 0; /* number of terms stored in the cache   */
static size_t cache_cap   = 0; /* capacity of the cache                */
static idx_t  next_n      = 0; /* next original n to test              */

seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* Compute n-th possible term (original n = next_n)   */
        seq_t denom = ( (seq_t)next_n ) * 8 + 28;   /* sum of n..n+7       */
        seq_t result = 1;                           /* product / denom   */

        for (idx_t k = 0; k < 8; ++k)
        {
            seq_t term = (seq_t)(next_n + k);
            /* Cancel common factors with denominator */
            seq_t g = gcd(term, denom);
            if (g > 1)
            {
                term /= g;
                denom /= g;
            }
            result *= term;
        }

        /* If denominator reduced to 1, the division was exact */
        if (denom == (seq_t)1)
        {
            /* Ensure space in cache */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache    = tmp;
                cache_cap= new_cap;
            }
            cache[cache_size++] = result;
        }
        ++next_n;
    }

    return cache[n - 1];
}

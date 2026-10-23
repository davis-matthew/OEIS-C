/*
 * A178529  Self-convolution square-root of A008977, where A008977(n) =
 *          (4n)!/(n!)^4.
 * https://oeis.org/A178529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n/(n!)^2 * Product_{k=0..n-1} (8k+1)*(8k+3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;       /* cached values */
static idx_t cache_len = 0;       /* number of values stored (next to compute) */
static size_t cache_cap = 0;      /* allocated array capacity */

static void extend_cache(idx_t n)
{
    /* nothing to do if already computed */
    if (cache_len > n)
        return;

    /* grow capacity to at least n+1, doubling each time */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;

    for (idx_t k = cache_len; k <= n; ++k)
    {
        if (k == 0)                     /* a(0) = 1 */
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev  = cache[k - 1];
        seq_t N     = prev;                               /* current numerator */
        seq_t denom = (seq_t)k * (seq_t)k;                /* current denominator */

        /* multiply by 4 */
        N *= (seq_t)4;
        seq_t g = gcd(N, denom);
        if (g != 1)
        {
            N /= g;
            denom /= g;
        }

        /* multiply by (8k-7) */
        seq_t f1 = (seq_t)(((idx_t)8 * k) - (idx_t)7);
        N *= f1;
        g = gcd(N, denom);
        if (g != 1)
        {
            N /= g;
            denom /= g;
        }

        /* multiply by (8k-5) */
        seq_t f2 = (seq_t)(((idx_t)8 * k) - (idx_t)5);
        N *= f2;
        g = gcd(N, denom);
        if (g != 1)
        {
            N /= g;
            denom /= g;
        }

        /* any remaining factors in denom are removed now */
        if (denom != 1)
            N /= denom;

        cache[k] = N;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}

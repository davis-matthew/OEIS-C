/*
 * A174315  a(n) = 3F0( -n,-n+1,-n+2;;-1)= n!*(n-1)!* 1F2(-n+2;2,3;-1)/2,
 *          where nFm(;;z) are generalized hypergeometric series.
 * https://oeis.org/A174315
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(3*n^2-9*n+7) - a(n-2)*3*(n-1)*(n-3)*(n-2)^2 +
 *            a(n-3)*(n-1)*(n-4)*(n-2)^2*(n-3)^2 with a(2)=1, a(3)=7, a(4)=97
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic cache array */
static idx_t cache_last = 0;         /* largest n computed and stored */

/* grow the cache to accommodate at least n+1 entries (0..n) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate initial array big enough for the first few terms */
        size_t init = (n < 10) ? 12 : (size_t)n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return;                    /* allocation failure – caller will not get a correct value */
        cache_last = 1;                         /* nothing populated yet */
    }
    if (n <= cache_last) return;              /* already have up to n */

    size_t newsize = cache_last + 1;
    if (newsize < n + 1) newsize = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) return;                         /* allocation failure – cannot extend cache */
    cache = tmp;

    /* initialize base values if they are inside the new range */
    if (cache_last < 2 && n >= 2) { cache[2] = (seq_t)1; cache_last = 2; }
    if (cache_last < 3 && n >= 3) { cache[3] = (seq_t)7; cache_last = 3; }
    if (cache_last < 4 && n >= 4) { cache[4] = (seq_t)97; cache_last = 4; }

    /* compute missing terms up to n */
    for (idx_t k = cache_last + 1; k <= n; ++k)
    {
        /* term1: (3*k^2-9*k+7) * a(k-1) */
        seq_t coeff1 = (seq_t)(3 * k * k - 9 * k + 7);
        seq_t term1  = coeff1 * cache[k - 1];

        /* term2: - 3*(k-1)*(k-3)*(k-2)^2 * a(k-2) */
        seq_t coeff2 = (seq_t)3 * (seq_t)(k - 1) * (seq_t)(k - 3) *
                       (seq_t)(k - 2) * (seq_t)(k - 2);
        seq_t term2  = coeff2 * cache[k - 2];

        /* term3: + (k-1)*(k-4)*(k-2)^2*(k-3)^2 * a(k-3) */
        seq_t coeff3 = (seq_t)(k - 1) * (seq_t)(k - 4) *
                       (seq_t)(k - 2) * (seq_t)(k - 2) *
                       (seq_t)(k - 3) * (seq_t)(k - 3);
        seq_t term3  = coeff3 * cache[k - 3];

        cache[k] = term1 - term2 + term3;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* values below the offset are undefined */
    ensure_cache(n);
    return cache[n];
}

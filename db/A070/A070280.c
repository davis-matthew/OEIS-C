/*
 * A070280  a(1) = 1; a(n) = a(n-1) + product of the digits of n-1.
 * https://oeis.org/A070280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + product_of_digits(n-1), a(1) = 1
 * Range:     u8 n <= 37, u16 n <= 868, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_product_idx.h"

static seq_t *cache = NULL;     /* cache[0] holds a(1) */
static idx_t cache_len = 0;     /* number of terms already stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure cache has capacity for at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap >= n) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap < (size_t)n) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, this is unused */
    if (n == 1) return 1;          /* a(1) = 1 */

    if ((idx_t)cache_len < n)
    {
        ensure_cache(n);
        /* compute terms from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t term;
            if (i == 1)
                term = 1;
            else
            {
                seq_t prod = digit_product_idx(i - 1);
                term = cache[i - 2] + prod;
            }
            cache[i - 1] = term;
        }
        cache_len = n;
    }
    return cache[n - 1];
}

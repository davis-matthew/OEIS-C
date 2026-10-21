/*
 * A090413  A Chebyshev transform of 3^n.
 * https://oeis.org/A090413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   3*(n+1)*a(n) - 8*(n+1)*a(n-1) + 12*(n-2)*a(n-2) +
 *            32*(2-n)*a(n-3) = 0 Recurrence (for n >= 3): a(n) = (
 *            8*(n+1)*a(n-1) - 12*(n-2)*a(n-2) + 32*(n-2)*a(n-3) ) / (3*(n+1))
 *            Initial values: a(0)=1, a(1)=3, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache    = NULL;
static size_t cache_cap = 0;
static idx_t  cache_max = 0;
static int    cache_init = 0;

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache_init)
    {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 3) init_cap = 3;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = init_cap;

        /* Base values */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 8;
        cache_max = 2;
        cache_init = 1;
    }

    /* Grow cache if needed */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return if already computed */
    if (n <= cache_max) return cache[n];

    /* Compute values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* All multiplications use seq_t coefficients; no signed underflow */
        seq_t term1 = 8 * ((seq_t)(i + 1)) * cache[i - 1];
        seq_t term2 = 12 * ((seq_t)(i - 2)) * cache[i - 2];
        seq_t term3 = 32 * ((seq_t)(i - 2)) * cache[i - 3];

        seq_t numer = term1 - term2 + term3;          /* = 8*(i+1)*a(i-1) -12*(i-2)*a(i-2) +32*(i-2)*a(i-3) */
        seq_t denom = 3 * ((seq_t)(i + 1));          /* = 3*(i+1) */

        cache[i] = numer / denom;                    /* division is exact */
    }

    cache_max = n;
    return cache[n];
}

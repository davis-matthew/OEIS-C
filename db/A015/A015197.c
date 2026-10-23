/*
 * A015197  Sum of Gaussian binomial coefficients for q=11.
 * https://oeis.org/A015197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 2 a(n) = 2*a(n-1) + (11^(n-1)-1) * a(n-2) for n
 *            >= 2 (Goldman + Rota, 1969)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* memoization table */
static size_t cache_cap = 0;     /* current capacity (number of slots) */

/* Ensure that cache has space for indices 0..need (inclusive)   */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need < cache_cap)
        return;                     /* already big enough */

    size_t new_cap = (size_t)need + 1;
    cache = seq_realloc(cache, cache_cap, new_cap);

    /* initialise any new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;

    cache_cap = new_cap;

    /* set the two base values if not already done */
    if (cache_cap > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;
    if (cache_cap > 1 && cache[1] == 0)
        cache[1] = (seq_t)2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    ensure_capacity(n);

    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] == 0)
        {
            seq_t a_prev  = cache[i - 1];
            seq_t a_prev2 = cache[i - 2];

            /* 11^(i-1) */
            seq_t pow_val = big_pow((seq_t)11, (seq_t)(i - 1));
            seq_t term = (pow_val - (seq_t)1) * a_prev2;

            cache[i] = (seq_t)2 * a_prev + term;
        }
    }
    return cache[n];
}

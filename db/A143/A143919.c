/*
 * A143919  Aliquot sequence starting at 100.
 * https://oeis.org/A143919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = A001065(a(n)), a(0) = 100
 * Range:     u8 n <= 0, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001065 is the sum of the proper divisors of its argument. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/isqrt_seq.h"

/* Compute the sum of proper divisors of x. */
static seq_t aliquot_sum(seq_t x)
{
    if (x <= (seq_t)1)
        return (seq_t)0;          /* 0 or 1 have no proper divisors */

    seq_t sum = (seq_t)1;         /* 1 is always a proper divisor if x>1 */
    seq_t sqrt_x = isqrt_seq(x);
    idx_t lim = (idx_t)sqrt_x;

    for (idx_t d = 2; d <= lim; ++d)
    {
        if (x % (seq_t)d == (seq_t)0)
        {
            seq_t other = x / (seq_t)d;
            sum += (seq_t)d;
            if (other != (seq_t)d)
                sum += other;
        }
    }
    return sum;
}

/* Memoized computation of the aliquot sequence starting from 100. */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of filled entries  */
    static idx_t cache_cap = 0;     /* allocated capacity     */

    /* Expand cache if necessary. */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failure – return 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fill cache up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)100;    /* initial value */
        else
        {
            seq_t prev = cache[i - 1];
            cache[i]  = prev == (seq_t)0 ? (seq_t)0 : aliquot_sum(prev);
        }
    }
    cache_len = n + 1;
    return cache[n];
}

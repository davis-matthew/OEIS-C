/*
 * A009444  E.g.f. log(1 + x*exp(-x)).
 * https://oeis.org/A009444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1) * n + sum_{k=1}^{n-1} (-1)^(n-k) *
 *            binomial(n-1,k-1) * (n-k) * a(k) a(0) = 0 , a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that cache has entries for indices 0..n inclusive. */
static void
ensure_cache(idx_t n)
{
    if ((idx_t)(n + 1) <= cache_cap)
        return; /* already large enough */

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    cache = new_cache;

    /* initialise base values if needed */
    if (cache_cap <= 0)
        cache[0] = (seq_t)0;
    if (cache_cap <= 1 && new_cap > 1)
        cache[1] = (seq_t)1;

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* handle the initial values directly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    idx_t old_cap = cache_cap;
    ensure_cache(n);

    /* compute values from the first missing index upward */
    for (idx_t m = (old_cap > 1 ? old_cap : 2); m <= n; ++m)
    {
        idx_t row = m - 1;          /* n-1 in the recurrence */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* binomial(row,0) */

        for (idx_t k = 1; k <= m - 1; ++k)
        {
            if (k > 1)
            {
                /* update binomial from binomial(row,k-2) to binomial(row,k-1) */
                binom = binom * (seq_t)(row - (k - 2)) / (seq_t)(k - 1);
            }

            seq_t term = binom * (seq_t)(m - k) * cache[k];
            if ((m - k) & 1)   /* (-1)^(n-k) = -1 when (n-k) is odd */
                sum -= term;
            else
                sum += term;
        }

        /* (-1)^(m+1) : even m -> -1, odd m -> +1 */
        seq_t sign = ((m & 1) == 0) ? (seq_t)(-1) : (seq_t)1;
        cache[m] = sum + sign * (seq_t)m;
    }

    return cache[n];
}

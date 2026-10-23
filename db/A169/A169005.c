/*
 * A169005  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169005: number of reduced words of length n in the Coxeter
   group with 40 generators and relations (S_i)^2 = (S_iS_j)^23 = I.
   The sequence satisfies the linear recurrence
        a(0) = 1
        a(n) = 2 + 38 * (a(0) + … + a(n-1))          for 1 ≤ n ≤ 22
        a(n) = 38 * (a(n-1) + … + a(n-22)) - 741 * a(n-23)   for n ≥ 23
   The implementation memoises computed values. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;      /* initialise new entries to 0 */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    idx_t old = cache_size;
    ensure_cache(n);

    for (idx_t i = old; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        if (i < 23)   /* 1 ≤ i ≤ 22 */
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)2 + (seq_t)38 * sum;
        }
        else          /* i ≥ 23 */
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 22; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)38 * sum - (seq_t)741 * cache[i - 23];
        }
    }

    return cache[n];
}

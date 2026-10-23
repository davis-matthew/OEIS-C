/*
 * A166616  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence a(0) = 1 a(n) = 2 + 27 *
 *            Σ_{k=0}^{n-1} a(k) for 1 ≤ n ≤ 11 a(12) = 1 + 27 * Σ_{k=1}^{11}
 *            a(k) – 378 * a(0) a(n) = 27 * Σ_{k=1}^{11} a(n-k) – 378 *
 *            a(n-12) for n ≥ 13
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache   = NULL;   /* array of a(n) values */
static size_t cache_cap = 0;    /* current allocated capacity */
static idx_t  cache_len = 0;    /* number of values that have been computed */

/* Ensure the cache can hold indices 0 … n */
static void
ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 16)
        new_cap = 16;                /* minimal initial capacity */
    if (new_cap < 2 * cache_cap)
        new_cap = 2 * cache_cap;    /* grow by at least a factor of two */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Public function to retrieve the n-th term (any order of calls). */
seq_t
a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cache_size(n);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
        {
            val = (seq_t)1;
        }
        else if (i <= 11)
        {
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];
            val = (seq_t)2 + (seq_t)27 * sum;
        }
        else if (i == 12)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 11; ++j)
                sum += cache[j];
            val = (seq_t)1 + (seq_t)27 * sum - (seq_t)378 * cache[0];
        }
        else   /* i >= 13 */
        {
            seq_t sum = 0;
            for (idx_t j = i - 1; j >= i - 11; --j)
                sum += cache[j];
            val = (seq_t)27 * sum - (seq_t)378 * cache[i - 12];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}

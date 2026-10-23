/*
 * A169027  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A169027: number of reduced words of length n in the Coxeter group
 * on 14 generators with (S_i)^2 = I and (S_i S_j)^24 = I.
 * The sequence satisfies the recurrence derived from its rational
 * generating function:
 *     a(0) = 1
 *     for 1 ≤ n ≤ 23:  a(n) = 2 + 12 * Σ_{k=0}^{n-1} a(k)
 *     for n = 24:      a(24) = 1 + 12 * Σ_{k=1}^{23} a(k) - 78 * a(0)
 *     for n ≥ 25:      a(n) = 12 * Σ_{k=n-23}^{n-1} a(k) - 78 * a(n-24)
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_len = 0;   /* number of precomputed elements */

/* Ensure the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    if (n < cached_len)
        return cache[(size_t)n];

    ensure_cache(n);

    for (idx_t i = cached_len; i <= n; ++i)
    {
        seq_t val;

        if (i <= 23)                 /* first 24 terms: linear in cumulative sum */
        {
            if (i == 0)
                val = (seq_t)1;
            else
            {
                seq_t sum_prev = 0;
                for (idx_t j = 0; j < i; ++j)
                    sum_prev += cache[(size_t)j];
                val = 2 + sum_prev * 12;
            }
        }
        else                          /* n ≥ 24: homogeneous linear recurrence */
        {
            /* sum over the last 23 terms */
            seq_t sum23 = 0;
            for (idx_t j = i - 1; ; --j)
            {
                sum23 += cache[(size_t)j];
                if (j == i - 23)
                    break;
            }
            val = sum23 * 12;
            if (i == 24)
                val += 1;            /* the non‑homogeneous term for n = 24 */
            val -= cache[(size_t)(i - 24)] * 78;
        }

        cache[(size_t)i] = val;
    }

    cached_len = n + 1;
    return cache[(size_t)n];
}

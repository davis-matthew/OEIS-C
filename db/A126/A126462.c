/*
 * A126462  Column 2 of triangle A126460; equals the number of subpartitions
 *          of the partition {(k^2 + 9*k + 20)*k/6, k>=0}.
 * https://oeis.org/A126462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   compute_pk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126462
   Formula: a(0)=1; for n>=1
   a(n)=1 - Σ_{k=0}^{n-1} a(k) * (-1)^{n-k} * binom(p_k, n-k)
   where p_k = ((k^2 + 9*k + 20) * k) / 6.
   The recurrence is evaluated iteratively and memoized in static arrays.
   All arithmetic is performed in type seq_t; no extra libraries are
   required beyond the standard C headers. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_pk.h"

static seq_t *a_cache = NULL;
static seq_t *bin_cache = NULL;
static seq_t *p_cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_upto = 0;

static void ensure_cache_size(idx_t n)
{
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        a_cache  = (seq_t *)realloc(a_cache,  new_cap * sizeof(seq_t));
        bin_cache = (seq_t *)realloc(bin_cache, new_cap * sizeof(seq_t));
        p_cache  = (seq_t *)realloc(p_cache,  new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ensure the cache can hold values up to n */
    ensure_cache_size(n);

    /* initialize a(0) if not yet */
    if (computed_upto == 0)
    {
        a_cache[0] = 1;
        bin_cache[0] = 1;
        p_cache[0] = 0;                 /* p_0 = 0 */
        computed_upto = 1;
    }

    /* compute values up to requested n */
    for (idx_t cur = (idx_t)computed_upto; cur <= n; ++cur)
    {
        if (cur == 0) continue;          /* already initialized */

        /* update binomial values for k = 0 .. cur-1
           bin_cache[k] will hold binom(p_k, cur - k) after this loop   */
        for (idx_t k = 0; k < cur; ++k)
        {
            idx_t old_r = (cur - 1) - k;          /* r before increment */
            seq_t pk = p_cache[k];
            if ((seq_t)old_r >= pk)
            {
                bin_cache[k] = 0;                 /* binom(p_k, r) = 0 for r > p_k */
                continue;
            }
            /* compute new binomial: bin = bin * (pk - old_r) / (old_r + 1) */
            seq_t mult = pk - (seq_t)old_r;
            bin_cache[k] = (bin_cache[k] * mult) / ((seq_t)old_r + 1);
        }

        /* compute a[cur] using the recurrence */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 0; k < cur; ++k)
        {
            seq_t term = bin_cache[k] * a_cache[k];
            if (((cur - k) & 1U) == 1U)
                sum_neg += term;
            else
                sum_pos += term;
        }
        a_cache[cur] = (sum_neg + 1U) - sum_pos;

        /* prepare bin_cache[cur] for the next iteration */
        bin_cache[cur] = 1;                 /* binom(p_cur, 0) = 1 */
        p_cache[cur] = compute_pk(cur);     /* store p_cur for future */
    }

    computed_upto = n + 1;
    return a_cache[n];
}

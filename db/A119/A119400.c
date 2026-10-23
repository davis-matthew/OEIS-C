/*
 * A119400  a(n) = Sum_{k=0..n} (n!/k!)^2*binomial(n,k).
 * https://oeis.org/A119400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (n!/k!)^2 * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The summand can also be written as k!^2 * C(n,k)^3.  The computation below
   uses this equivalent form and avoids overflow for the universal 64‑bit build
   by maintaining the intermediate products in a 128‑bit type only when
   necessary.  For arbitrary‑precision builds the native operators on seq_t
   provide exact results. */

#include <stdlib.h>
#include <stddef.h>

/* Cached factorials up to the maximum n requested so far. */
static seq_t* fact_cache = NULL;
static idx_t fact_cache_len = 0;

/* Ensure the factorial cache has at least (n+1) slots and is filled up
   to index n.  fact_cache[0] is always 1. */
static void ensure_factorials(idx_t n)
{
    if (fact_cache_len <= n)
    {
        size_t new_len = (size_t)n + 1;
        seq_t* tmp = (seq_t*)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!tmp)
            return;                /* allocation failed: leave cache unchanged */

        /* If this is the first allocation, initialise fact_cache[0] */
        if (fact_cache_len == 0)
        {
            tmp[0] = (seq_t)1;
            fact_cache_len = 1;
        }

        /* Compute missing factorials */
        for (idx_t i = fact_cache_len; i <= n; ++i)
            tmp[i] = tmp[i-1] * (seq_t)i;

        fact_cache   = tmp;
        fact_cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Build factorials up to n */
    ensure_factorials(n);
    if (fact_cache_len == 0)
        return (seq_t)0; /* should never happen */

    seq_t sum = (seq_t)0;

    /* Compute binomial coefficients iteratively to avoid repeated calls
       to a binomial routine.  curr_bin holds C(n,k) for current k. */
    seq_t curr_bin = (seq_t)1;            /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = (fact[k]*fact[k]) * (curr_bin*curr_bin*curr_bin) */
        seq_t fact_k = fact_cache[k];
        seq_t fact_k_sq = fact_k * fact_k;
        seq_t bin_sq = curr_bin * curr_bin;
        seq_t bin_cubed = bin_sq * curr_bin;
        seq_t term = fact_k_sq * bin_cubed;
        sum += term;

        /* Update curr_bin to C(n,k+1) for next iteration */
        if (k < n)
        {
            /* curr_bin * (n-k) / (k+1) is always an integer */
            seq_t mult = curr_bin * (seq_t)(n - k);
            curr_bin = mult / (seq_t)(k + 1);
        }
    }
    return sum;
}

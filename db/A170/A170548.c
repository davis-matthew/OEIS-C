/*
 * A170548  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   reduced words of length n in the Coxeter group with 11
 *            generators * (S_i)^2 = I and (S_i S_j)^47 = I. * * The
 *            generating function is * (t^47 + 2t^46 + 2t^45 + ··· + 2t + 1) /
 *            * (1 - 9t - 9t^2 - ··· - 9t^46 + 45t^47), * so the sequence
 *            satisfies the linear recurrence * * a_n =
 *            9·(a_{n-1}+…+a_{n-46...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cached sequence values          */
static idx_t  cache_cap = 0;   /* allocated capacity of `cache`    */
static idx_t  cache_len = 0;   /* number of values actually stored */

/* Ensure the cache can hold values up to index n (inclusive).  */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)        /* already computed up to n      */
        return;

    /* grow capacity if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= n)
            new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;

        /* 9 * (a_{i-1} + … + a_{i-46})  */
        for (idx_t k = 1; k <= 46 && k <= i; ++k)
            sum += cache[i - k];
        seq_t term = sum * (seq_t)9;

        /* -45 * a_{i-47}  for i ≥ 47   */
        if (i >= 47)
            term -= cache[i - 47] * (seq_t)45;

        /* additive constant p_i   */
        seq_t p = 0;
        if (i == 0)
            p = 1;
        else if (i <= 46)
            p = 2;
        else if (i == 47)
            p = 1;

        cache[i] = term + p;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n) {          /* required function */
    ensure_cache(n);
    return cache[n];
}

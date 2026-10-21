/*
 * A098799  a(n) = Sum_{k>=0} k^n*A000045(k)/2^(k+1).
 * https://oeis.org/A098799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} binomial(n,k)*(2+2^(n-k))*a(k); a(0) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation with memoization using the recurrence. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *cache          = NULL;   /* array holding computed a(k) */
static idx_t  cache_cap      = 0;      /* allocated capacity (in elements) */
static idx_t  computed_len   = 0;      /* number of entries that are filled */

/* make sure cache can hold indices 0..n */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = (size_t)cache_cap ? (size_t)cache_cap : (size_t)8;
    while ((size_t)new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    if (tmp) cache_cap = (idx_t)new_cap;
}

/* return the nth term of A098799 */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* compute missing terms up to n */
    for (idx_t i = computed_len; i <= n; ++i)
    {
        if (i == (idx_t)0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)1;        /* the "+ 1" in the recurrence */

        for (idx_t k = (idx_t)0; k < i; ++k)
        {
            seq_t bin   = binomial(i, k);            /* binomial(n,k) */
            seq_t pow   = pow2(i - k);               /* 2^(n-k)   */
            seq_t coeff = pow + (seq_t)2;            /* 2 + 2^(n-k) */
            seq_t term  = bin;
            term *= coeff;
            term *= cache[k];
            sum += term;
        }

        cache[i] = sum;
    }

    computed_len = n + (idx_t)1;   /* all terms up to n are filled */
    return cache[n];
}

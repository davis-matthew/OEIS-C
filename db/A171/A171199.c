/*
 * A171199  G.f. satisfies: A(x) = exp( Sum_{n>=1} [A(x)^n + A(x)^-n]*x^n/n ).
 * https://oeis.org/A171199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(n)=a(n-1)+a(n-2)+∑_{k=2}^{n-1} a(k)*a(n-k-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"

/* static cache for memoization */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of terms stored (next free index) */
static size_t cache_cap = 0;    /* current capacity */

/* ensure that cache contains terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* nothing to do if we already have enough terms */
    if (cache_len > n) return;

    /* first allocation: make sure we have at least space for the requested n+1 terms */
    if (cache_len == 0) {
        /* allocate minimal capacity: at least 2 (for a0 and a1) or more if n is larger */
        size_t cap = (n + 1 > 2) ? (n + 1) : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_cap = cap;
        /* initialise known terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* grow the array if necessary */
    if (cache_len <= (size_t)n && cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort; in the context of these exercises
               this situation is unlikely */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        /* for i==0 or 1 we already have the values, but the loop below
           would handle them correctly as well; nevertheless we skip for
           efficiency */
        if (i >= 2) {
            for (idx_t k = 2; k <= i - 1; ++k) {
                /* sum of product a(k) * a(i-k-1) */
                seq_t prod = seq_mul(cache[k], cache[i - k - 1]);
                sum += prod;
            }
            cache[i] = cache[i - 1] + cache[i - 2] + sum;
        }
    }
    cache_len = n + 1;
}

/* public interface demanded by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    ensure_cache(n);
    return cache[n];
}

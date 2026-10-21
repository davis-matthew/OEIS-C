/*
 * A202829  Expansion of e.g.f.: exp(4*x/(1-3*x)) / sqrt(1-9*x^2).
 * https://oeis.org/A202829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202829: Expansion of e.g.f.: exp(4*x/(1-3*x)) / sqrt(1-9*x^2)
   Recurrence: a(n) = (3*n+1)*a(n-1) + 3*(n-1)*(3*n+1)*a(n-2)
                - 27*(n-1)*(n-2)^2*a(n-3),  a(0)=1, a(1)=4, a(2)=49 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t cache_len = 0;      /* first unused index (cache_len-1 is last valid) */

/* Ensure the cache holds at least 'need'+1 elements. */
static void ensure_cache_size(idx_t need)
{
    if (need + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)need + 1)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise with the first three terms (necessary for the recurrence). */
static void init_cache(void)
{
    if (cache != NULL) return;     /* already initialised */
    ensure_cache_size(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)49;
    cache_len = 3;
}

/* Compute a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)
        return cache[n];

    ensure_cache_size(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)4;
        else if (i == 2)
            val = (seq_t)49;
        else
        {
            seq_t nseq = (seq_t)i;

            seq_t term1 = ((seq_t)3 * nseq + 1) * cache[i-1];

            seq_t coef2 = (seq_t)3 * ((seq_t)(i-1));
            coef2 *= ((seq_t)3 * nseq + 1);
            seq_t term2 = coef2 * cache[i-2];

            seq_t coef3 = (seq_t)27 * ((seq_t)(i-1));
            coef3 *= ((seq_t)(i-2));
            coef3 *= ((seq_t)(i-2));
            seq_t term3 = coef3 * cache[i-3];

            val = term1 + term2 - term3;   /* unsigned arithmetic handles underflow */
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

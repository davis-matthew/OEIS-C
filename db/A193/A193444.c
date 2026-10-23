/*
 * A193444  E.g.f.: exp( Sum_{n>=1} n!*x^(2*n)/(2*n)! ) = Sum_{n>=0}
 *          a(n)*x^(2*n)/(2*n)!.
 * https://oeis.org/A193444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} binomial(2n-1,2k-1) * k! * a(n-k), with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises all values up to any requested n,
   using the recurrence only.  */

#include <stdlib.h>

static seq_t *cache = NULL;          /* a[0] .. a[n]            */
static idx_t  cache_max = 0;         /* highest index computed   */

static seq_t *fact_tbl = NULL;       /* factorial values 0!..   */
static idx_t  fact_max  = 0;         /* biggest factorial stored */

/* ensure that cache holds indices 0..n, initialise a[0] = 1 if needed */
static void ensure_cache_size(idx_t n)
{
    if (!cache) {
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * 16);
        if (!cache) abort();
        for (idx_t i = 0; i < 16; ++i) cache[i] = 0;
        cache[0] = (seq_t)1;
        cache_max = 0;
    }
    if (n < (idx_t)(sizeof(seq_t)*cache_max)) return;

    size_t required = (size_t)n + 1;
    size_t cur = (size_t)sizeof(seq_t) * 16;
    if (required <= cur) return;
    while (cur < required) cur <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cur * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
}

/* ensure fact_tbl holds 0! .. n! */
static void ensure_factorial_size(idx_t n)
{
    if (!fact_tbl) {
        fact_tbl = (seq_t *)realloc(NULL, sizeof(seq_t) * 16);
        if (!fact_tbl) abort();
        for (idx_t i = 0; i < 16; ++i) fact_tbl[i] = 0;
        fact_tbl[0] = (seq_t)1;
        fact_max = 0;
    }
    if (n <= fact_max) return;

    size_t cur = fact_max + 1;
    size_t req = n + 1;
    while (cur < req) cur <<= 1;
    seq_t *tmp = (seq_t *)realloc(fact_tbl, cur * sizeof(seq_t));
    if (!tmp) abort();
    fact_tbl = tmp;
    for (idx_t i = fact_max + 1; i <= n; ++i)
        fact_tbl[i] = fact_tbl[i - 1] * (idx_t)i;
    fact_max = n;
}

/* main routine ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    ensure_factorial_size(n);

    if (n <= cache_max) return cache[n];

    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            /* compute binomial(2*m-1, 2*k-1) */
            seq_t bin = 1;
            idx_t r  = 2 * k - 1;
            idx_t nn = 2 * m - 1;
            for (idx_t t = 1; t <= r; ++t) {
                bin *= (seq_t)(nn - (t - 1));
                bin /= (seq_t)t;
            }
            /* term = bin * k! * a(m-k) */
            seq_t term = bin;
            term *= fact_tbl[k];
            term *= cache[m - k];
            sum += term;
        }
        cache[m] = sum;
    }

    cache_max = n;
    return cache[n];
}

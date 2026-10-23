/*
 * A166482  a(n) = Sum_{k=0..n} binomial(n+k,2*k)*Fibonacci(2*k+1).
 * https://oeis.org/A166482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,2*k) * Fibonacci(2*k+1).
 *            Recurrence: a(n) = 7*a(n-1) - 13*a(n-2) + 7*a(n-3) - a(n-4) for
 *            n ≥ 4 Initial values: a(0)=1, a(1)=3, a(2)=12, a(3)=51
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0..current_max] */
static idx_t  cache_len = 0;    /* number of initialized entries */

/* Initialize cache with first four values. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache = (seq_t *)malloc(4 * sizeof(seq_t));
    /* a(0) ... a(3) */
    cache[0] = 1;
    cache[1] = 3;
    cache[2] = 12;
    cache[3] = 51;
    cache_len = 4;
}

/* Extend cache up to n (inclusive).  */
static void extend_cache(idx_t n)
{
    if (n < cache_len)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – terminate program as this sequence cannot be
           produced without memory. */
        abort();
    }
    cache = new_cache;
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i - 1] * 7;            /* 7*a(i-1) */
        term -= cache[i - 2] * 13;                /* -13*a(i-2) */
        term += cache[i - 3] * 7;                 /* +7*a(i-3) */
        term -= cache[i - 4];                     /* -a(i-4) */
        cache[i] = term;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n >= cache_len)
        extend_cache(n);
    return cache[(size_t)n];
}

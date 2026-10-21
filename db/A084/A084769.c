/*
 * A084769  a(n) = P_n(9), where P_n is n-th Legendre polynomial; also, a(n) =
 *          central coefficient of (1 + 9*x + 20*x^2)^n.
 * https://oeis.org/A084769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   multdiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084769
 * Recurrence:  a(0)=1, a(1)=9,
 *              n*a(n)+9*(1-2*n)*a(n-1)+(n-1)*a(n-2)=0   (n>=2)
 * (equivalently  a(n) = [9*(2*n-1)*a(n-1) - (n-1)*a(n-2)] / n)
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/multdiv.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Cache of previously computed terms.
 * cache[i] contains a(i) for 0 <= i < cache_len. */
static seq_t *cache      = NULL;   /* array of a(i)              */
static idx_t cache_len   = 0;      /* number of valid entries    */
static idx_t cache_cap   = 0;      /* allocated array size       */

/* ------------------------------------------------------------ */
/* Grow the cache array to accommodate at least 'needed+1' indices. */
static void grow_cache(idx_t needed)
{
    if (needed < cache_cap) return;

    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 2;
    if (new_cap < needed + 1) new_cap = needed + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failed */

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)9;

    /* initialise cache on first call */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_len = 2;
        cache_cap = 2;
    }

    /* extend cache up to index n (if necessary) */
    if (n >= cache_len) {
        grow_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* term1 = 9*(2*i-1)*a(i-1) / i */
            unsigned long long mult1 = 9ULL * (2ULL * i - 1ULL);
            seq_t term1 = multdiv(cache[i - 1], mult1, i);

            /* term2 = (i-1)*a(i-2) / i */
            seq_t term2 = multdiv(cache[i - 2], i - 1ULL, i);

            cache[i] = term1 - term2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}

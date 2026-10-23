/*
 * A123087  Sequence of numbers such that a(2*n) + a(n) = n and a(n) is the
 *          smallest number such that a(n) >= a(n-1).
 * https://oeis.org/A123087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2) - a(floor(n/2)), with a(0)=0 and
 *            a(2k+1)=a(2k). This implements an iterative, memoised version
 *            that works for any supported seq_t and idx_t
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding computed values */
static idx_t  cached_n = 0;    /* highest index already computed (inclusive) */
static size_t cap = 0;         /* allocated array size */

/* Ensure the cache holds indices 0..n */
static void ensure_cache(idx_t n)
{
    if (n < cached_n) return;              /* nothing to do */
    if (cap <= (size_t)n) {                /* need to grow */
        size_t newcap = (size_t)n + 1;
        if (newcap < 8) newcap = 8;        /* minimum */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }
    /* compute new entries */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t half = i >> 1;                /* floor(i/2) */
        cache[i] = (seq_t)half - cache[half];
    }
    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}

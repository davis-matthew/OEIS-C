/*
 * A188479  Binomial variation of the Catalan numbers: a(n) = sum(
 *          binomial(n-i, i)*a(i)*a(n-1-i), i=0..(n-1) ) with a(0)=1.
 * https://oeis.org/A188479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{n-1} binomial(n-1-i,i) * a(i) * a(n-1-i), a(0)
 *            = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* NULL, size_t */
#include <stdlib.h>            /* malloc, realloc, exit */
#include "../utils/binom.h"    /* binom(idx_t a, idx_t b) */


static seq_t *cache = NULL;   /* memoised a(0)…a(… ) */
static size_t cache_cap = 0;  /* allocated array size */
static idx_t  up_to = (idx_t)-1;   /* largest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to n.  Grow exponentially. */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                       /* already large enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {                       /* allocation failed */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    grow_cache(n);

    /* initialise base case on first use */
    if (up_to == (idx_t)-1) {
        cache[0] = (seq_t)1;
        up_to = 0;
    }

    if (n <= up_to)                 /* value already computed */
        return cache[n];

    /* compute values progressively up to n */
    for (idx_t m = up_to + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= m - 1; ++i) {
            idx_t j   = m - 1 - i;
            seq_t bin = binom(m - i, i);          /* binomial coefficient */
            seq_t prod = bin * cache[i] * cache[j];
            sum += prod;
        }
        cache[m] = sum;
    }

    up_to = n;
    return cache[n];
}

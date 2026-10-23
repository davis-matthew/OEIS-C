/*
 * A041459  Denominators of continued fraction convergents to sqrt(245).
 * https://oeis.org/A041459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Denominator of the nth convergent of the continued
 *            fraction of sqrt(245) Recurrence: a(n) = 103682*a(n-10) -
 *            a(n-20); a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=23, a(5)=141,
 *            a(6)=1010, a(7)=1151, a(8)=2161, a(9)=3312. We compute the terms
 *            directly via the continued‑fraction recurrence: q(-1)=0,...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned a_period[10] = {1,1,1,7,6,7,1,1,1,30};

seq_t a(idx_t n)
{
    /* Cache of computed terms for fast repeated queries. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of elements currently stored */

    if (cache == NULL) {
        /* Initialise cache with the first term a(0)=1. */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Ensure the cache can hold indices up to n. */
    if (cache_len <= n) {
        /* Expand cache. */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
    }

    /* Compute new terms from the first missing index up to n. */
    seq_t prev1 = 0, prev2 = 0;           /* q(i-1) and q(i-2) for the current i */
    if (cache_len >= 1) prev1 = cache[cache_len - 1];
    if (cache_len >= 2) prev2 = cache[cache_len - 2];

    for (idx_t i = cache_len; i <= n; ++i) {
        unsigned ai = a_period[(i - 1) % 10];      /* partial quotient a_i */
        seq_t val = ai * prev1 + prev2;            /* q(i) = a_i*q(i-1)+q(i-2) */
        cache[i] = val;
        prev2 = prev1;
        prev1 = val;
    }

    cache_len = n + 1;
    return cache[n];
}

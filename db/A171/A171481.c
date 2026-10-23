/*
 * A171481  a(n) = a(a(n-1)) + a(n - a(n-1) - 2) with a(0) = 0, a(1) = a(2) =
 *          1.
 * https://oeis.org/A171481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-1)) + a(n - a(n-1) - 2), a(0)=0, a(1)=a(2)=1
 * Range:     u8 n <= 936, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoization array */
static size_t cap     = 0;           /* allocated capacity */
static size_t last    = (size_t)-1;  /* last index whose value is known */

/* compute the n‑th term of OEIS A171481 */
seq_t a(idx_t n)
{
    /* initialise cache with the first three values */
    if (cache == NULL) {
        size_t init = 3;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* out of memory */
        cap = init;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last   = 2;
    }

    /* grow the array if we need to address a larger index */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* out of memory */
        /* zero‑initialize the new slots (optional) */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* if the value is already known, return it */
    if ((size_t)n <= last)
        return cache[(size_t)n];

    /* compute the missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t prev  = cache[i - 1];        /* a(i-1) */
        /* a(prev) – prev is guaranteed ≤ i-2 in the sequence */
        size_t idx1 = (size_t)((idx_t)prev);
        seq_t term1 = cache[idx1];

        /* second index: i - prev - 2 */
        size_t idx2 = (size_t)(((idx_t)i) - ((idx_t)prev) - 2);
        seq_t term2 = cache[idx2];

        cache[i] = term1 + term2;
    }
    last = n;
    return cache[(size_t)n];
}

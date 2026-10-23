/*
 * A021053  Decimal expansion of 1/49.
 * https://oeis.org/A021053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence gives the decimal digits of 1/49,
   a(0)=0, a(1)=2, a(2)=0, a(3)=4, …, repeating every 42 digits.
   Computed by long division:
      r_0 = 1;
      a(n) = (r_n * 10) / 49,
      r_{n+1} = (r_n * 10) % 49.
   Only integer arithmetic is used, so it works for all seq_t sizes. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *cache = NULL;           /* array of seq_t values */
    static idx_t cached_len = 0;          /* number of cached elements */
    static seq_t rem = (seq_t)1;          /* remainder r_n for the last computed n */

    /* if we already have the requested term, just return it */
    if (n < cached_len)
        return cache[n];

    /* enlarge cache to hold terms up to n */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – return 0 as a fallback (should not happen) */
        return (seq_t)0;
    }
    cache = new_cache;

    /* compute missing terms from cached_len to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t digit = (rem * (seq_t)10) / (seq_t)49;
        rem = (rem * (seq_t)10) % (seq_t)49;
        cache[i] = digit;
    }
    cached_len = new_len;

    return cache[n];
}

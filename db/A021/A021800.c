/*
 * A021800  Decimal expansion of 1/796.
 * https://oeis.org/A021800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n (starting from 0) of the decimal expansion of
 *            1/796
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th decimal digit of 1/796 using long‑division.
    The function caches results so subsequent calls are O(1) for
    previously computed digits.  The cache is built on demand and
    grows incrementally.  The remainder after each step is kept in
    a 64‑bit variable – it never exceeds 795.  The cache itself holds
    the digits as seq_t values. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;   /* array of computed digits */
    static size_t cache_len = 0;  /* number of digits stored */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static uint64_t remainder = 1;/* remainder before next digit */

    const uint64_t divisor = 796ULL;

    /* If n is already cached, return it directly */
    if (n < cache_len)
        return cache[n];

    /* First call: allocate an initial cache */
    if (cache == NULL) {
        cache_cap = (n + 1 > 64) ? (n + 1) : 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        remainder = 1; /* start with numerator 1 before the decimal point */
    }

    /* Compute digits up to index n */
    while (cache_len <= n) {
        uint64_t prod  = remainder * 10ULL;   /* shift remainder left by one decimal digit */
        uint64_t digit = prod / divisor;      /* next decimal digit */
        remainder = prod % divisor;           /* new remainder for the next step */

        /* Grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len] = (seq_t)digit;
        cache_len++;
    }
    return cache[n];
}

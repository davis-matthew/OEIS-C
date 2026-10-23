/*
 * A021083  Decimal expansion of 1/79.
 * https://oeis.org/A021083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/79) mod 10
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth decimal digit of 1/79 (a(0) = 0) */
seq_t a(idx_t n)
{
    /* static cache for computed digits */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of valid entries in cache   */
    static seq_t rem = 1;            /* remainder after the last digit     */

    /* If the requested index is already cached, just return it */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate index n (next element is n) */
    size_t needed = (size_t)(n + 1);
    if (!cache) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_len = 0;
        rem = 1;                     /* initial remainder before first digit */
    } else if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    }

    /* Compute missing digits using long‑division algorithm */
    for (idx_t i = cache_len; i <= n; ++i) {
        rem = rem * (seq_t)10;             /* multiply current remainder by 10 */
        seq_t digit = rem / (seq_t)79;     /* next decimal digit             */
        rem = rem % (seq_t)79;             /* update remainder               */
        cache[i] = digit;                  /* store computed digit           */
    }

    cache_len = needed;                     /* all indices 0..n are now cached */
    return cache[n];
}

/*
 * A021440  Decimal expansion of 1/436.
 * https://oeis.org/A021440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor(10^(n+1)/436) mod 10  (nth digit of the decimal expansion of 1/436) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t rem_cache = 1;   /* remainder after generating cache_size digits */

    if (n < cache_size)
        return cache[n];

    /* Expand cache to hold digits up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed; return 0 as a safe fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* Generate remaining digits using long‑division algorithm */
    for (idx_t i = cache_size; i <= n; ++i) {
        rem_cache *= 10;
        cache[i] = (seq_t)(rem_cache / 436);
        rem_cache = rem_cache % 436;
    }

    cache_size = new_len;
    return cache[n];
}

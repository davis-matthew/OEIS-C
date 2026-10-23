/*
 * A021587  Decimal expansion of 1/583.
 * https://oeis.org/A021587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/583) mod 10 (n-th decimal digit of 1/583)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t remainder = 1;  /* remainder after generating digit cache_len‑1 */

    if (cache == NULL) {
        cache_cap = 4;                    /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        remainder = 1;
    }

    while ((idx_t)cache_len <= n) {
        if (cache_len == cache_cap) {     /* grow the array */
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Generate the next decimal digit */
        remainder = remainder * 10;
        seq_t digit = (seq_t)(remainder / 583);   /* 0 … 9 */
        remainder = remainder % 583;
        cache[cache_len++] = digit;
    }

    return cache[n];
}

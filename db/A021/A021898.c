/*
 * A021898  Decimal expansion of 1/894.
 * https://oeis.org/A021898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth decimal digit of 1/894
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already-computed digits */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of digits in cache   */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */
    static seq_t remainder   = 1;   /* remainder before the next digit */

    /* enlarge cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate digits up to index n */
    while (cache_len <= n) {
        seq_t digit = (remainder * (seq_t)10) / (seq_t)894;
        cache[cache_len++] = digit;
        remainder = (remainder * (seq_t)10) % (seq_t)894;
    }

    return cache[n];
}

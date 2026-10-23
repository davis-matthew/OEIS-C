/*
 * A021909  Decimal expansion of 1/905.
 * https://oeis.org/A021909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit of the decimal expansion of 1/905 after the
 *            decimal point (a(0) is the first digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits (0..9) */
    static seq_t *cache = NULL;      /* array of digits */
    static size_t cache_len = 0;     /* number of digits computed so far */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t remainder = 1;      /* remainder for long division */

    /* If we already have the requested digit, return it. */
    if (n < cache_len)
        return cache[n];

    /* Allocate or grow the cache if needed. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* fallback on allocation failure */
    }
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate digits from the end of the cache up to index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t tmp = remainder * 10ULL;   /* remainder * 10 */
        seq_t digit = (seq_t)(tmp / 905ULL);  /* next decimal digit */
        cache[i] = digit;
        remainder = (idx_t)(tmp % 905ULL);   /* new remainder */
    }
    cache_len = n + 1;

    return cache[n];
}

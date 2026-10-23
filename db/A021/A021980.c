/*
 * A021980  Decimal expansion of 1/976.
 * https://oeis.org/A021980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   digits of the decimal expansion of 1/976. a(0) is the first
 *            digit after the decimal point. The value at position n is
 *            produced by long division: r_0 = 1, digit_n = floor(r_n * 10 /
 *            976), r_{n+1} = r_n * 10 % 976
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* stored digits */
    static size_t cache_len = 0; /* number of digits stored */
    static size_t cache_cap = 0; /* allocated capacity */
    static uint64_t rem = 1;     /* remainder after cache_len digits */

    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow cache to at least n+1 elements */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        /* avoid overflow of allocation size */
        if (new_cap > SIZE_MAX / sizeof(seq_t))
            new_cap = SIZE_MAX / sizeof(seq_t);

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new digits up to position n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        uint64_t prod = rem * 10ULL;      /* prod < 10 * 976 < 10000 */
        seq_t digit = (seq_t)(prod / 976ULL);
        rem = prod % 976ULL;
        cache[i] = digit;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}

/*
 * A201272  Number of n X 3 0..2 arrays with every row and column
 *          nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) - 4*a(n-5) + 2*a(n-7) -
 *            2*a(n-9) + 4*a(n-11) + a(n-12) - 2*a(n-13) - 2*a(n-14) +
 *            a(n-16), a(0)=1, a(1)=1, a(2)=4, a(3)=7, a(4)=14, a(5)=21,
 *            a(6)=41, a(7)=54, a(8)=86, a(9)=120, a(10)=168, a(11)=218,
 *            a(12)=307, a(13)=377, a(14)=496, a(15)=621
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 910, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values for indices 0 .. 15 */
    static const seq_t init_vals[16] = {
        (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)7,
        (seq_t)14, (seq_t)21, (seq_t)41, (seq_t)54,
        (seq_t)86, (seq_t)120, (seq_t)168, (seq_t)218,
        (seq_t)307, (seq_t)377, (seq_t)496, (seq_t)621
    };

    /* static cache for already computed values */
    static seq_t *cache      = NULL; /* pointer to array of seq_t */
    static size_t cache_len = 0;     /* number of elements stored */

    /* initialise cache once */
    if (cache_len == 0) {
        cache_len = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* if the requested value is already cached, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to include the requested index */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);

    /* compute terms from current length up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        /* compute the positive and negative parts separately
           to avoid intermediate underflows when seq_t is unsigned */
        seq_t pos = ((seq_t)2 * cache[i-2]) + ((seq_t)2 * cache[i-3]) +
                    ((seq_t)2 * cache[i-7]) + ((seq_t)4 * cache[i-11]) +
                    cache[i-12] + cache[i-16];

        seq_t neg = cache[i-4] + ((seq_t)4 * cache[i-5]) +
                    ((seq_t)2 * cache[i-9]) + ((seq_t)2 * cache[i-13]) +
                    ((seq_t)2 * cache[i-14]);

        cache[i] = pos - neg;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}

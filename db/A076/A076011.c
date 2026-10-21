/*
 * A076011  Fifth column of triangle A075504.
 * https://oeis.org/A076011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * S2(n+5,5) = 135*a(n-1) - 6885*a(n-2) +
 *            164025*a(n-3) - 1797714*a(n-4) + 7085880*a(n-5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap*2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialize first five terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)135;
        cache[2] = (seq_t)11340;
        cache[3] = (seq_t)765450;
        cache[4] = (seq_t)45605511;
        cache_len = 5;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 5) continue;     /* already set */

        seq_t term = (seq_t)135 * cache[i-1];
        term += (seq_t)164025 * cache[i-3];
        term += (seq_t)7085880 * cache[i-5];

        seq_t subtract = (seq_t)6885 * cache[i-2];
        subtract += (seq_t)1797714 * cache[i-4];

        term -= subtract;
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

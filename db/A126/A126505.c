/*
 * A126505  Number of base 10 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) + 3*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5) for n>5
 *            Initial values: a(0)=1, a(1)=10, a(2)=70, a(3)=510, a(4)=3720,
 *            a(5)=27142
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache large enough for index n */
    if ((size_t)n + 1 > cache_len) {
        size_t needed = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory; caller can ignore */
        cache = tmp;

        size_t old_len = cache_len;
        cache_len = needed;

        /* Initialize the first six terms (if space exists) */
        if (cache_len > 0) cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)10;
        if (cache_len > 2) cache[2] = (seq_t)70;
        if (cache_len > 3) cache[3] = (seq_t)510;
        if (cache_len > 4) cache[4] = (seq_t)3720;
        if (cache_len > 5) cache[5] = (seq_t)27142;

        /* Compute missing values using the recurrence */
        size_t start = old_len;
        if (start < 6) start = 6;
        for (size_t i = start; i < cache_len; ++i) {
            seq_t term = 7 * cache[i - 1] + 3 * cache[i - 2] + cache[i - 5];
            term -= 6 * cache[i - 3];
            term -= cache[i - 4];
            cache[i] = term;
        }
    }

    return cache[(size_t)n];
}

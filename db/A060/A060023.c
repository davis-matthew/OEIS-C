/*
 * A060023  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 4.
 * https://oeis.org/A060023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10),
 *            with a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=0, a(6)=1,
 *            a(7)=-1, a(8)=-1, a(9)=-3
 * Range:     i8 n <= 26, i16 n <= 167, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init[10] = {1, 0, 1, 1, 1, 0, 1, -1, -1, -3};

    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failed */

        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 10) {
                new_cache[i] = init[i];
            } else {
                new_cache[i] = new_cache[i-1] + new_cache[i-2]
                               - 2 * new_cache[i-5]
                               + new_cache[i-8]
                               + new_cache[i-9]
                               - new_cache[i-10];
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[(size_t)n];
}

/*
 * A197049  Number of n X 3 0..4 arrays with each element equal to the number
 *          its horizontal and vertical zero neighbors.
 * https://oeis.org/A197049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) - a(n-4) - a(n-5) for n>6
 *            Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=10, a(4)=18,
 *            a(5)=38, a(6)=78
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)10,
        (seq_t)18,
        (seq_t)38,
        (seq_t)78
    };

    if (n < 7)
        return init[n];

    static seq_t *cache = NULL;
    static size_t max_cached = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)7 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* memory allocation failed */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        max_cached = 6;
    }

    if ((size_t)n > max_cached) {
        size_t new_max = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* memory allocation failed */
        cache = tmp;

        for (size_t i = max_cached + 1; i <= new_max; ++i) {
            seq_t val = cache[i - 1] + cache[i - 2]
                      + ((seq_t)3 * cache[i - 3])
                      - cache[i - 4]
                      - cache[i - 5];
            cache[i] = val;
        }
        max_cached = new_max;
    }

    return cache[(size_t)n];
}

/*
 * A071121  a(n) = a(n-1) + sum of decimal digits of n^3.
 * https://oeis.org/A071121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + sum of decimal digits of n^3 a(1) = 1
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache that keeps the already computed a(k) values.
       cache[i] holds a(i+1), so index 0 corresponds to a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0, cache_cap = 0;

    /* Guard for n < 1: the sequence has offset 1, but calls below
       should not request n == 0. */
    if (n == 0)
        return 0;

    /* Initialise cache if necessary */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;        /* allocation failure (unlikely) */
        /* a(1) = 1: sum of digits of 1^3 */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Expand cache to cover the requested n */
    if (n > cache_len) {
        idx_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap = (new_cap == 0) ? 1 : new_cap * 2;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure (unlikely) */
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Compute cube of i as 64‑bit (fits for i <= 10^4). */
            uint64_t cube = (uint64_t)i * i * i;

            /* Sum decimal digits of cube */
            uint64_t temp = cube;
            uint32_t digit_sum = 0;
            while (temp > 0) {
                digit_sum += (uint32_t)(temp % 10);
                temp /= 10;
            }

            /* Append to previous value */
            cache[cache_len] = cache[cache_len - 1] + (seq_t)digit_sum;
            ++cache_len;
        }
    }

    /* Return the cached answer */
    return cache[n - 1];
}

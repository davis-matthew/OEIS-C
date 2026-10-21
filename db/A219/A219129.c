/*
 * A219129  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..2 2 X n array.
 * https://oeis.org/A219129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) – 6*a(n-3) + 7*a(n-4) – 10*a(n-5) +
 *            6*a(n-6) – 6*a(n-7) – 4*a(n-8) Initial terms (for n = 1 … 8): 9,
 *            15, 87, 205, 899, 2521, 9709, 29747
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initial values */
    static const seq_t init[8] = {
        (seq_t)9, (seq_t)15, (seq_t)87, (seq_t)205,
        (seq_t)899, (seq_t)2521, (seq_t)9709, (seq_t)29747
    };

    /* Ensure the cache is allocated and holds at least the first 8 terms */
    if (cache_size < 8) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend the cache up to n using the 8‑term recurrence */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    for (size_t i = cache_size; i < n; ++i) {
        /* Fetch the last eight values (a(i), a(i-1), ..., a(i-7)) */
        seq_t a1 = cache[i - 1];   /* a(i)   */
        seq_t a2 = cache[i - 2];   /* a(i-1) */
        seq_t a3 = cache[i - 3];   /* a(i-2) */
        seq_t a4 = cache[i - 4];   /* a(i-3) */
        seq_t a5 = cache[i - 5];   /* a(i-4) */
        seq_t a6 = cache[i - 6];   /* a(i-5) */
        seq_t a7 = cache[i - 7];   /* a(i-6) */
        seq_t a8 = cache[i - 8];   /* a(i-7) */

        /* Compute the recurrence: positive part first */
        seq_t pos = a1 * 2   + a2 * 6   + a4 * 7   + a6 * 6;
        seq_t neg = a3 * 6   + a5 * 10  + a7 * 6   + a8 * 4;

        /* pos is guaranteed to be >= neg for this sequence */
        cache[i] = pos - neg;
    }
    cache_size = n;
    return cache[n - 1];
}

/*
 * A185265  a(0)=1, a(1)=2; thereafter a(n) = f(n-1) + f(n-2) where f() =
 *          A164387().
 * https://oeis.org/A185265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)+a(n-5),
 *            a(0)=1,a(1)=2,a(2)=3,a(3)=6,a(4)=12,a(5)=22
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 5 */
    static const seq_t base[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)6, (seq_t)12, (seq_t)22 };

    if (n < 6)
        return base[n];

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
    }

    /* Ensure the cache covers index n */
    if ((idx_t)cache_size <= n) {
        size_t new_size = cache_size ? cache_size : 1;
        while ((idx_t)new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 4] + cache[i - 5];

    return cache[n];
}

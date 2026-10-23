/*
 * A108356  Count, repeating multiples of 3 four times, all other numbers
 *          twice.
 * https://oeis.org/A108356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-8) - a(n-9), with a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=2, a(4)=3, a(5)=3, a(6)=3, a(7)=3, a(8)=4
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to support arbitrary (non‑monotonic) call order. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize the cache with the first nine terms. */
    if (cache_size == 0)
    {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        cache[0] = (seq_t)1;  cache[1] = (seq_t)1;  cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;  cache[4] = (seq_t)3;  cache[5] = (seq_t)3;
        cache[6] = (seq_t)3;  cache[7] = (seq_t)3;  cache[8] = (seq_t)4;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to include term n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 8] - cache[i - 9];

    cache_size = new_size;
    return cache[n];
}

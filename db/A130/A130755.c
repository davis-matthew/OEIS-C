/*
 * A130755  Binomial transform of periodic sequence (3, 1, 2).
 * https://oeis.org/A130755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) a(0) = 3, a(1) = 4, a(2) =
 *            7 Recurrence holds for n > 2
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small values returned directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;   /* number of allocated slots   */
    static size_t cached_upto = 2;      /* largest index that has been computed */

    /* Initialise cache with the first three values */
    if (cache_capacity == 0)
    {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
        cache_capacity = 3;
        cached_upto   = 2;
    }

    /* If the requested index is already cached, return it */
    if (n <= cached_upto)
        return cache[n];

    /* Ensure the cache has enough space */
    if ((size_t)n + 1 > cache_capacity)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* In a production build we should check for a NULL result,
           but the exercise guarantees sufficient memory. */
        cache = tmp;
        cache_capacity = n + 1;
    }

    /* Compute values up to n using the recurrence.
       The intermediate sum 3*cache[i-1] + 2*cache[i-3]
       is always greater than 3*cache[i-2], so unsigned
       arithmetic does not underflow. */
    for (idx_t i = cached_upto + 1; i <= n; ++i)
    {
        seq_t t1 = cache[i-1] * (seq_t)3;
        seq_t t3 = cache[i-3] * (seq_t)2;
        seq_t t2 = cache[i-2] * (seq_t)3;
        cache[i] = t1 + t3 - t2;
    }

    cached_upto = n;
    return cache[n];
}

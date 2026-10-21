/*
 * A055263  a(n) = Sum of digits of (n + a(n-1)).
 * https://oeis.org/A055263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum of digits of (n + a(n-1))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the digit sum of an unsigned 64‑bit integer. */
static inline seq_t digit_sum_uint64(uint64_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated slots */
    static size_t computed = 0;   /* highest index + 1 computed */

    /* Allocate the cache on the first call. */
    if (cache == NULL)
    {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure → return 0 */
            return (seq_t)0;
        cache[0] = 0;               /* a(0) = 0 */
        computed = 1;
    }

    /* Grow the cache if an index beyond the current capacity is requested. */
    if ((size_t)n + 1 > capacity)
    {
        capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (tmp == NULL)           /* reallocation failure → return 0 */
            return (seq_t)0;
        cache = tmp;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = computed; i <= (size_t)n; ++i)
    {
        uint64_t sum_big = (uint64_t)i + (uint64_t)cache[i - 1];
        cache[i] = digit_sum_uint64(sum_big);
    }
    computed = (size_t)n + 1;

    return cache[n];
}

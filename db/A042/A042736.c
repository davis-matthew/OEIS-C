/*
 * A042736  Numerators of continued fraction convergents to sqrt(898).
 * https://oeis.org/A042736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1798*a(n-4) - a(n-8) a(0)=29, a(1)=30, a(2)=869,
 *            a(3)=899, a(4)=53011, a(5)=53910, a(6)=1562491, a(7)=1616401
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Helper: multiply by 1798 with 128‑bit intermediate when needed. */
static seq_t mul_1798(seq_t x)
{
#if defined(__cplusplus)
    /* C++17 path: use type_traits to detect integral types. */
    if constexpr (std::is_integral<seq_t>::value)
    {
#if defined(__SIZEOF_INT128__)
        unsigned __int128 prod = (unsigned __int128)1798 * (unsigned __int128)x;
        return (seq_t)prod;
#else
        return 1798 * x;
#endif
    }
    else
    {
        return 1798 * x;
    }
#else
    /* C11 path: rely on __int128 support if available. */
#if defined(__SIZEOF_INT128__)
    return (seq_t)((unsigned __int128)1798 * (unsigned __int128)x);
#else
    return 1798 * x;
#endif
#endif
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialize the cache on the first call */
    if (!cache)
    {
        cache_cap = 8;
        cache_len = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort(); /* allocation failure */

        cache[0] = (seq_t)29;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)869;
        cache[3] = (seq_t)899;
        cache[4] = (seq_t)53011;
        cache[5] = (seq_t)53910;
        cache[6] = (seq_t)1562491;
        cache[7] = (seq_t)1616401;
    }

    /* If the requested index is already computed, return it. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Extend the cache up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        /* Grow the array if necessary. */
        if (i + 1 >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t prod = mul_1798(cache[i - 4]);
        cache[i] = prod - cache[i - 8];
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}

/*
 * A092236  a(n) = (3^n + 2*3^(n/2)*cos(n*Pi/6))/3.
 * https://oeis.org/A092236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 2*3^(n/2)*cos(n*Pi/6)) / 3 A linear recurrence
 *            derived from the generating function: a(n) = 6*a(n-1) -
 *            12*a(n-2) + 9*a(n-3) (n >= 3) with initial values a(0)=1,
 *            a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

static inline seq_t
compute_term(seq_t a1, seq_t a2, seq_t a3)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral_v<seq_t> && !std::is_same_v<seq_t, unsigned __int128>) {
        __uint128_t tmp = (__uint128_t)a1 * 6;
        tmp -= (__uint128_t)a2 * 12;
        tmp += (__uint128_t)a3 * 9;
        return (seq_t)tmp;
    } else {
        return a1 * 6 - a2 * 12 + a3 * 9;
    }
#else
    return a1 * 6 - a2 * 12 + a3 * 9;
#endif
}

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* compute a[i] using the recurrence */
            cache[i] = compute_term(cache[i-1], cache[i-2], cache[i-3]);
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}

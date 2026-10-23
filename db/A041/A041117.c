/*
 * A041117  Denominators of continued fraction convergents to sqrt(67).
 * https://oeis.org/A041117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 97684 * a(n-10) - a(n-20) for n >= 20 a(0) = 1, a(1) = 5,
 *            a(2) = 11, a(3) = 16, a(4) = 27, a(5) = 205, a(6) = 232, a(7) =
 *            437, a(8) = 1106, a(9) = 5967, a(10)= 96578, a(11)= 488857,
 *            a(12)= 1074292, a(13)= 1563149, a(14)= 2637441, a(15)= 20025236,
 *            a(16)= 22662677, a(17)= 42687913, a(18)= 10...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[20] = {
    1ULL, 5ULL, 11ULL, 16ULL, 27ULL,
    205ULL, 232ULL, 437ULL, 1106ULL, 5967ULL,
    96578ULL, 488857ULL, 1074292ULL, 1563149ULL,
    2637441ULL, 20025236ULL, 22662677ULL, 42687913ULL,
    108038503ULL, 582880428ULL
};

static seq_t *cache = NULL;
static size_t   cache_cap = 0;
static size_t   cache_len = 0;

seq_t a(idx_t n)
{
    /* Quick return for the first twenty terms. */
    if (n < 20)
        return INIT[(size_t)n];

    /* Ensure the cache exists and holds the initial ten terms. */
    if (cache_len == 0) {
        cache_len = 20;
        cache_cap = 20;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 20; ++i)
            cache[i] = INIT[i];
    }

    /* Expand the cache to cover all required indices up to n. */
    if ((size_t)n >= cache_len) {
        idx_t  needed = n + 1;
        if ((size_t)needed > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)needed > new_cap)
                new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t k = cache_len; k <= n; ++k)
            cache[(size_t)k] =
                (seq_t)97684 * cache[(size_t)(k - 10)]
                - cache[(size_t)(k - 20)];

        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

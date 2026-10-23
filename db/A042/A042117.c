/*
 * A042117  Denominators of continued fraction convergents to sqrt(583).
 * https://oeis.org/A042117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 16859086*a(n-10) - a(n-20) for n > 19, * with the
 *            following initial values: * a(0)=1, a(1)=6, a(2)=7, a(3)=55,
 *            a(4)=282, a(5)=1183, * a(6)=6197, a(7)=44562, a(8)=50759,
 *            a(9)=349116, * a(10)=16808327, a(11)=101199078, a(12)=118007405,
 *            * a(13)=927250913, a(14)=4754261970, a(15)=19944298793,...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 13, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache    = NULL;   // array holding the terms
    static size_t  cap     = 0;      // current capacity of cache
    static size_t  last   = 0;      // highest index already computed
    static bool    init    = false;   // whether the initial 20 terms are in the cache

    /* Ensure capacity for at least 20 terms when needed */
    if (!init) {
        size_t need = 20;
        if (cap < need)
            grow_seq_array(&cache, &cap, need);

        /* initial values (as listed in OEIS) */
        static const seq_t init_vals[20] = {
            1ULL, 6ULL, 7ULL, 55ULL, 282ULL,
            1183ULL, 6197ULL, 44562ULL, 50759ULL, 349116ULL,
            16808327ULL, 101199078ULL, 118007405ULL,
            927250913ULL, 4754261970ULL, 19944298793ULL,
            104475755935ULL, 751274590338ULL,
            855750346273ULL, 5885776667976ULL
        };

        for (size_t i = 0; i < 20; ++i)
            cache[i] = init_vals[i];
        last = 19;
        init = true;
    }

    /* If the requested index is already computed, return it. */
    if ((size_t)n <= last)
        return cache[(size_t)n];

    /* Make sure the cache is large enough. */
    if ((size_t)n >= cap)
        grow_seq_array(&cache, &cap, (size_t)n + 1);

    /* Compute new terms from the first missing index up to n. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t high = cache[i - 10];   /* a(i-10) is guaranteed to be computed */
        seq_t low  = cache[i - 20];   /* a(i-20) is guaranteed to be computed */

#ifdef __cplusplus
        /* For arbitrary‑precision seq_t, simple arithmetic is sufficient. */
        cache[i] = (seq_t)16859086 * high - low;
#else
        /* For fixed‑width integer seq_t, use 128‑bit intermediate to avoid overflow. */
        __int128 prod = (__int128)16859086 * (__int128)high;
        __int128 sub  = prod - (__int128)low;
        cache[i] = (seq_t)sub;
#endif
    }

    last = (size_t)n;
    return cache[(size_t)n];
}

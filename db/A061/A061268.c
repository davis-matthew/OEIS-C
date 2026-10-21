/*
 * A061268  Numbers k such that k^2 has property that the sum of its digits
 *          and the product of its digits are nonzero squares.
 * https://oeis.org/A061268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that k^2 has non‑zero digit sum and
   non‑zero digit product, both perfect squares.
   The function enumerates such k steadily, caching all found
   values.  It uses only standard C operations and a few helpers
   that are available in the build environment.  No external
   libraries are required beyond the standard <math.h>.  The
   routine is O(n) with respect to the index requested. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/digit_sum_prod.h"

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)). */
static inline unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long r = (unsigned long long)sqrtl((long double)x);
    /* Correct possible rounding errors */
    while ((unsigned __int128)r * r > x) --r;
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* Test whether an unsigned __int128 value is a perfect square. */
static inline bool is_square128(unsigned __int128 x)
{
    unsigned long long r = isqrt128(x);
    return ((unsigned __int128)r * r) == x;
}

/* Main sequence function: a(n) = k where k^2 has both
   digit sum and digit product non‑zero perfect squares. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t size = 0;       /* number of cached terms */
    static size_t cap = 0;        /* current cache capacity */
    static idx_t next_k = 1;      /* next k to test */

    /* Grow cache until we have computed at least n terms */
    while (size < n)
    {
        /* Compute k^2 as a 128‑bit integer (no overflow for k up to ~10^19) */
        unsigned __int128 sq = (unsigned __int128)next_k * (unsigned __int128)next_k;

        /* Sum and product of digits of k^2 */
        unsigned __int128 sum, prod;
        digit_sum_prod(sq, &sum, &prod);

        /* Ignore zero sum or product */
        if (sum == 0 || prod == 0) { ++next_k; continue; }

        /* Check that both are perfect squares */
        if (!(is_square128(sum) && is_square128(prod))) { ++next_k; continue; }

        /* Store the qualifying k in the cache */
        if (size == cap)
        {
            cap = (cap == 0 ? 16 : cap * 2);
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort gracefully */
                free(cache);
                size = 0;
                cap = 0;
                return (seq_t)0;
            }
            cache = tmp;
        }
        cache[size++] = (seq_t)next_k;  /* cast is safe: k <= 2^64-1 */

        ++next_k;
    }

    return cache[n - 1];   /* a(1) is stored at index 0 */
}

/*
 * A074236  Numbers that are the long leg of some integer right triangle and
 *          the hypotenuse of some other integer right triangle.
 * https://oeis.org/A074236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   _gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A074236: intersection of long legs and hypotenuses
   of integer right triangles.  The generated elements are stored
   in a static cache, and a(n) returns the n‑th element (n ≥ 1).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_gcd_u64.h"

/* Euclidean GCD for 64‑bit unsigned integers.  */

/* Static cache of the sequence values.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Generate all intersection values up to a given numeric limit.  */
static void _generate_up_to(uint64_t limit)
{
    bool *long_flag = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    bool *hyp_flag  = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!long_flag || !hyp_flag) return;              /* allocation failure */

    uint64_t m_max = 0;
    while ((m_max + 1) * (m_max + 1) <= limit) ++m_max;

    for (uint64_t m = 2; m <= m_max; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;            /* same parity */
            if (_gcd_u64(m, n) != 1) continue;           /* not coprime */

            seq_t a0 = (seq_t)m * m - (seq_t)n * n;      /* m^2 - n^2 */
            seq_t b0 = (seq_t)2 * m * n;                 /* 2mn */
            seq_t long0 = a0 > b0 ? a0 : b0;              /* long leg of primitive triple */
            seq_t c0   = (seq_t)m * m + (seq_t)n * n;    /* hypotenuse of primitive triple */

            seq_t k = 1;
            while (true) {
                seq_t val_long = long0 * k;
                if (val_long > limit) break;
                size_t idx_long = (size_t)val_long;
                long_flag[idx_long] = true;

                seq_t val_hyp = c0 * k;
                if (val_hyp <= limit)
                    hyp_flag[(size_t)val_hyp] = true;

                ++k;
            }
        }
    }

    /* Collect the intersection set in ascending order. */
    for (size_t i = 1; i <= (size_t)limit; ++i) {
        if (long_flag[i] && hyp_flag[i]) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return;                     /* realloc failed */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)i;
        }
    }

    free(long_flag);
    free(hyp_flag);
}

/* Ensure the cache holds at least 'need' elements.  */
static void _ensure_cache(idx_t need)
{
    if ((idx_t)cache_len >= need) return;

    /* Start with a reasonable limit and double until enough. */
    uint64_t limit = (uint64_t)need * 20;
    if (limit < 100) limit = 100;

    while ((idx_t)cache_len < need) {
        /* Invalidate any previous cache. */
        free(cache);
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;

        _generate_up_to(limit);

        if ((idx_t)cache_len >= need) break;
        limit *= 2;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, defensive */
    _ensure_cache(n);
    return cache[n - 1];
}

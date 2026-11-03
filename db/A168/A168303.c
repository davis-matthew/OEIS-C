/*
 * A168303  The fifth left hand column of triangle A167552.
 * https://oeis.org/A168303
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (642*n^9 - 13833*n^8 + 132840*n^7 - 726642*n^6 +
 *            2439738*n^5 - 5133177*n^4 + 6699660*n^3 - 5194188*n^2 +
 *            2157840*n - 362880) / 9! (valid for n >= 5)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 26, u64 n <= 281, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 974 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (order 10) for n >= 15:
   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 252*a(n-5)
          -210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10)   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute a(n) for n <= 14 using the polynomial form, using 64‑bit signed
   arithmetic which is safe for n <= 14. */
static seq_t poly_base_value(idx_t n)
{
    int64_t nn = (int64_t)n;
    int64_t n1 = nn;
    int64_t n2 = n1 * n1;
    int64_t n3 = n2 * n1;
    int64_t n4 = n3 * n1;
    int64_t n5 = n4 * n1;
    int64_t n6 = n5 * n1;
    int64_t n7 = n6 * n1;
    int64_t n8 = n7 * n1;
    int64_t n9 = n8 * n1;

    int64_t num = 642 * n9
                  - 13833 * n8
                  + 132840 * n7
                  - 726642 * n6
                  + 2439738 * n5
                  - 5133177 * n4
                  + 6699660 * n3
                  - 5194188 * n2
                  + 2157840 * n1
                  - 362880;

    /* division by 9! = 362880, the numerator is always divisible */
    return (seq_t)(num / 362880);
}

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_size = 0;        /* number of initialized entries in cache */
static size_t cache_cap = 0;         /* allocated capacity */

/* Coefficients of the recurrence (positive first, negative second) */
static const int coeff[10] = {10, -45, 120, -210, 252, -210, 120, -45, 10, -1};

/* Ensure that the cache has room for at least indices up to n. */
static void _ensure_cache_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n < 16) ? 16 : n + 1;
        cache = mem_new(seq_t, cache_cap);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    } else if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (n + 1 > new_cap) new_cap <<= 1;   /* double until enough */
        cache = mem_resize(seq_t, cache, new_cap);
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* Main routine computing a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Offset is 5, below that the definition is undefined – return 0. */
    if (n < 5) return 0;

    _ensure_cache_capacity(n);

    /* Compute missing terms up to n, starting where we left off. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i <= 14) {
            /* Base values via polynomial (valid for n ≤ 14). */
            val = poly_base_value(i);
        } else {
            /* Use the linear recurrence.  Separate positive and negative
               contributions to avoid underflow on unsigned types. */
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 1; j <= 10; ++j) {
                int c = coeff[j - 1];
                seq_t term = cache[i - j];
                if (c > 0)
                    pos += (seq_t)c * term;
                else
                    neg += (seq_t)(-c) * term;
            }
            val = pos - neg;   /* pos ≥ neg by construction */
        }
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}

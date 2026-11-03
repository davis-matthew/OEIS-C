/*
 * A048563  a(n+1) is next smallest nontrivial cube beginning with a(n),
 *          initial cube is 1.
 * https://oeis.org/A048563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_i32, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A048563:
   a(n+1) is the smallest cube > a(n) whose decimal representation starts
   with the digits of a(n) and is not a trivial repetition of a(n) with
   trailing zeros.  a(0) = 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* make sure we have room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialize the first term */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prefix = cache[i-1];
        seq_t pow10  = (seq_t)1;  /* 10^d, starts at d = 0 */

        seq_t next_cube;
        for (;;) {
            /* bounds for cubes that start with 'prefix' */
            seq_t lower = seq_mul(prefix, pow10);                      /* prefix*10^d   */
            seq_t upper = seq_mul(prefix + (seq_t)1, pow10);          /* (prefix+1)*10^d */
            upper -= (seq_t)1;                                        /* minus one      */

            /* smallest integer k with k^3 >= lower */
            seq_t k = floor_cube_root(lower);
            seq_t cube = ipow_idx(k, (idx_t)3);
            if (cube < lower) {
                ++k;
                cube = ipow_idx(k, (idx_t)3);
            }

            /* ensure cube is strictly greater than prefix*10^d */
            if (cube <= lower) {
                ++k;
                cube = ipow_idx(k, (idx_t)3);
            }

            /* if cube fits the interval and is > lower, we found it */
            if (cube <= upper) {
                next_cube = cube;
                break;
            }

            /* otherwise increase the number of trailing zeros searched */
            pow10 = mul_small(pow10, 10);
        }

        cache[i] = next_cube;
    }

    cache_size = n + 1;
    return cache[n];
}

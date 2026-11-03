/*
 * A117687  Squares for which both the sum of the digits and the product of
 *          the digits are cubes.
 * https://oeis.org/A117687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 3445, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9831 (2026-11-03)
 * Helpers:   digit_product, digit_sum, ipow_i32, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117687: squares whose digit sum and digit product are cubes (brute‑force). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Return true iff x is a perfect cube. */
static inline bool is_cube(seq_t x)
{
    if (x == 0) return true;
    seq_t r = floor_cube_root(x);
    return ipow_idx(r, (idx_t)3) == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard for out‑of‑range */

    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t next_m = 0;           /* next integer to be squared */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t m = (seq_t)next_m;
        seq_t square = seq_mul(m, m);

        seq_t sum = digit_sum(square);
        seq_t prod = digit_product(square);

        if (is_cube(sum) && is_cube(prod))
            append_candidate(&cache, &cache_len, &cache_cap, square);

        ++next_m;
    }

    return cache[n - 1];
}

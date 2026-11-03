/*
 * A074188  Smallest power (>=2) >= n!.
 * https://oeis.org/A074188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest perfect power >= n! (exponents ≥2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   ipow_i32, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Compute the sequence A074188 */
seq_t a(idx_t n)
{
    /* Cache of factorials up to the largest n requested (max 10000) */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cap = 0;          /* largest index computed so far */

    /* Allocate cache once (size 10001 to hold 0…10000) */
    if (!fact_cache) {
        size_t max_n = 10000;
        fact_cache = seq_alloc_new_array(max_n + 1);
        fact_cache[0] = 1;           /* 0! = 1 */
        fact_cap = 0;
    }

    /* Extend factorial cache up to n if necessary */
    if (n > fact_cap) {
        for (idx_t i = fact_cap + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;  /* factorial[i] = factorial[i-1] * i */
        }
        fact_cap = n;
    }

    seq_t F = fact_cache[n];          /* F = n! */

    /* Smallest square >= F */
    seq_t t = floor_sqrt(F);
    seq_t sq = t * t;
    if (sq < F) {
        t = t + (seq_t)1;
        sq = t * t;
    }

    /* Smallest cube >= F */
    seq_t c = floor_cube_root(F);
    seq_t cb = ipow_idx(c, 3);
    if (cb < F) {
        c = c + (seq_t)1;
        cb = ipow_idx(c, 3);
    }

    /* Return the smaller of the two candidates */
    seq_t ans = sq;
    if (cb < ans) ans = cb;
    return ans;
}

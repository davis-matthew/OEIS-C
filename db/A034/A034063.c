/*
 * A034063  Decimal part of a(n)^(1/8) starts with n (8th powers excluded).
 * https://oeis.org/A034063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest integer m that is not an 8th power * and
 *            whose eighth root has a decimal part starting with n * (prefix
 *            of length = number of decimal digits of n)
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: compute integer floor of the eighth root of x (x >= 0). */
static inline idx_t floor_root_8(idx_t x)
{
    if (x == 0) return 0;
    idx_t low = 0;
    idx_t high = 1;
    /* Find an upper bound. */
    while (((unsigned __int128)high * high * high * high *
            high * high * high * high) <= x)
        high <<= 1;
    while (high - low > 1) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 pow8 = ((unsigned __int128)mid * mid * mid * mid *
                                 mid * mid * mid * mid);
        if (pow8 <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Cached results.  cache[n] stores a(n);  0 means not yet computed. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Return the number of decimal digits of an unsigned integer. */
static inline int digit_count_idx(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Extend cache if necessary. */
    if (n >= cache_size) {
        size_t new_size = (n + 1) * 2;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache) abort(); /* no memory */
        cache = new_cache;
        /* Initialize new elements to 0. */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }
    if (cache[n] != 0) /* already computed */
        return cache[n];

    /* Number of digits in n (k).  For n==0, treat as 1 digit. */
    int k = digit_count_idx(n);
    long double mult = 1.0L;
    for (int i = 0; i < k; ++i)
        mult *= 10.0L;                     /* 10^k as long double */

    /* Scan m from 1 upward until we find the required one. */
    for (idx_t m = 1;; ++m) {
        /* Skip 8th powers. */
        idx_t r = floor_root_8(m);
        if (((unsigned __int128)r * r * r * r *
             r * r * r * r) == m)
            continue;

        long double f = powl((long double)m, 1.0L / 8.0L);
        idx_t int_part = (idx_t)f;
        long double frac = f - (long double)int_part;
        idx_t prefix = (idx_t)(frac * mult + 1e-12L);  /* floor */

        if (prefix == n) {
            cache[n] = (seq_t)m;
            return cache[n];
        }
    }
}

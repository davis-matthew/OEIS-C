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
 * Helpers:   floor_root_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/floor_root_8.h"

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
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
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

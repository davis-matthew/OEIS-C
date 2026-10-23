/*
 * A117051  Enneagonal numbers whose sum of digits is also enneagonal.
 * https://oeis.org/A117051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 4568, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117051: Enneagonal numbers whose sum of digits is also enneagonal.
   Implementation: iterate through enneagonal numbers E(m) = m*(7*m-5)/2,
   store those whose digit sum is enneagonal.  Results are cached for
   fast repeated calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* integer square root for uint64_t, returns floor(sqrt(x)) */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    for (int bit = 62; bit >= 0; --bit) {
        uint64_t tmp = r | (1ULL << bit);
        if (tmp <= x / tmp) r = tmp;
    }
    return r;
}

/* test whether s is an enneagonal number (s >= 0) */
static bool is_enneagonal(uint64_t s)
{
    if (s == 0) return true;                /* 0 is treated as enneagonal */
    uint64_t D = 25ULL + 56ULL * s;         /* discriminant 7k^2 - 5k - 2s = 0 */
    uint64_t sqrtD = isqrt_uint64(D);
    if (sqrtD * sqrtD != D) return false;   /* D must be a perfect square    */
    return ((5ULL + sqrtD) % 14ULL) == 0ULL; /* only the positive root needed */
}

/* cached results of the sequence */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap =   0;
static idx_t next_m   =   0;  /* next m to test (starting from 0) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, but handle zero gracefully */

    while (cache_size < n) {
        uint64_t m      = (uint64_t)next_m;
        uint64_t val_u  = m * (7 * m - 5) / 2;          /* E(m) in 64‑bit */
        seq_t val       = (seq_t)val_u;                /* safe when fits */

        /* digit sum of val, may need larger type for comparison */
        uint64_t ds_u   = (uint64_t)digit_sum(val);
        if (is_enneagonal(ds_u)) {
            if (cache_size >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation failed */
                cache = tmp;
            }
            cache[cache_size++] = val;
        }
        ++next_m;
    }

    return cache[n - 1];
}

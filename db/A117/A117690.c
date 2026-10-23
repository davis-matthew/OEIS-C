/*
 * A117690  Cubes for which both the sum of the digits and the product of the
 *          digits are squares.
 * https://oeis.org/A117690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 109, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117690: cubes k³ whose digit sum and digit product are perfect squares.
   The function builds the list lazily and returns the n‑th element
   (offset 1). */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Helper: digit sum of a 64‑bit number.                                 */
static seq_t digit_sum_uint64(uint64_t v)
{
    seq_t sum = 0;
    if (v == 0) return 0;
    while (v != 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Helper: digit product of a 64‑bit number.  The product fits in 64 bits
   for all cubes that occur in the search space. */
static seq_t digit_product_uint64(uint64_t v)
{
    if (v == 0) return 0;
    uint64_t prod = 1;
    while (v != 0) {
        uint32_t d = v % 10;
        if (d == 0) return 0;
        prod *= d;
        v /= 10;
    }
    return prod;
}

/* Helper: integer square root of a 64‑bit number.
   Returns the largest r such that r*r <= x. */
static seq_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;  /* 2^32 > sqrt(2^64-1) */
    while (lo <= hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return (seq_t)hi;
}

/* --------------------------------------------------------------------- */
/* Memoized cache of sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = (idx_t)-1;   /* last cube root examined */

/* Expand the cache until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
        last_k   = (idx_t)-1;
    }

    while ((idx_t)cache_len < n) {
        ++last_k;                         /* new k → cube = k³         */
        __int128 cube128 = (__int128)last_k * last_k * last_k;

        /* For very large cubes that do not fit into uint64_t, stop
           because further terms will exceed the capacity of seq_t.   */
        if (cube128 > (__int128)UINT64_MAX) {
            break;
        }

        uint64_t cube_val = (uint64_t)cube128;

        seq_t sum = digit_sum_uint64(cube_val);
        seq_t sqrt_sum = floor_sqrt_uint64((uint64_t)sum);
        if (sqrt_sum * sqrt_sum != sum) continue;      /* sum not square */

        seq_t prod = digit_product_uint64(cube_val);
        seq_t sqrt_prod = floor_sqrt_uint64((uint64_t)prod);
        if (sqrt_prod * sqrt_prod != prod) continue;   /* prod not square */

        /* Append the cube to the cache. */
        if (cache_len == cache_cap) {
            size_t newcap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = newcap;
        }
        cache[cache_len++] = (seq_t)cube_val;
    }
}

/* --------------------------------------------------------------------- */
/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of bounds, undefined */
    ensure_terms(n);
    return cache[n - 1];
}

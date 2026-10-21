/*
 * A221115  Number of 3 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without 2-loops.
 * https://oeis.org/A221115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221115: Number of 3 × n arrays of occupancy after each element
   moves to some horizontal or antidiagonal neighbor, without 2‑loops.
   Empirical recurrence (for n > 15):
   a(n) = 24*a(n-1) –144*a(n-2) + 46*a(n-3) + 1135*a(n-4) –908*a(n-5)
          –3663*a(n-6) +2438*a(n-7) +6528*a(n-8) –4340*a(n-9)
          –5525*a(n-10) +5164*a(n-11) –674*a(n-12) –528*a(n-13) –32*a(n-14)
   Initial values:
   a(1)=0,a(2)=0,a(3)=8,a(4)=178,a(5)=2952,a(6)=45842,a(7)=693362,
   a(8)=10369358,a(9)=154285828,a(10)=2290284810,a(11)=33961683620,
   a(12)=503356596054,a(13)=7458718434538,a(14)=110511504732170,
   a(15)=1637306438141992. */

#include <stdlib.h>

/* The first 15 terms; element 0 unused for 1‑based indexing. */
static const unsigned long long init_vals64[16] = {
    0ULL, /* 0th */
    0ULL, /* 1 */
    0ULL, /* 2 */
    8ULL, /* 3 */
    178ULL, /* 4 */
    2952ULL, /* 5 */
    45842ULL, /* 6 */
    693362ULL, /* 7 */
    10369358ULL, /* 8 */
    154285828ULL, /* 9 */
    2290284810ULL, /* 10 */
    33961683620ULL, /* 11 */
    503356596054ULL, /* 12 */
    7458718434538ULL, /* 13 */
    110511504732170ULL, /* 14 */
    1637306438141992ULL /* 15 */
};

/* Coefficients for the recurrence (14 terms). */
static const int coeffs[14] = {
    24, -144, 46, 1135, -908, -3663,
    2438, 6528, -4340, -5525, 5164,
    -674, -528, -32
};

/* Cache for computed values, 1‑based indices. */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* Highest n computed so far (≥15 after init). */
static size_t cache_capacity = 0;  /* Allocated size (≥ cache_size+1). */

/* Ensure the cache can hold indices up to need (inclusive). */
static void ensure_capacity(size_t need)
{
    if (need + 1 <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Initialize cache with the first 15 terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_capacity = 16;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    for (size_t i = 0; i < cache_capacity; ++i)
        cache[i] = 0;
    for (size_t i = 1; i <= 15; ++i)
        cache[i] = (seq_t)init_vals64[i];
    cache_size = 15;
}

seq_t a(idx_t n)
{
    if (n <= 15)
        return (seq_t)init_vals64[(size_t)n];

    init_cache();

    size_t target = (size_t)n;
    if (target > cache_size) {
        ensure_capacity(target);
        for (size_t i = cache_size + 1; i <= target; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 14; ++k) {
                idx_t idx = i - (k + 1);
                seq_t term = cache[idx];
                int c = coeffs[k];
                if (c >= 0) {
                    pos = pos + (seq_t)c * term;
                } else {
                    neg = neg + (seq_t)(-c) * term;
                }
            }
            cache[i] = pos - neg;
        }
        cache_size = target;
    }

    return cache[target];
}

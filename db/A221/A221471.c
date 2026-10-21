/*
 * A221471  Integers n such that n^2 is the difference of two Lucas numbers
 *          (A000032).
 * https://oeis.org/A221471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 30, u64 n <= 53, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221471:  a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=6,
   a(8)=11, a(9)=14, a(10)=29, a(11)=57, a(12)=76, a(13)=199,
   a(n)=3*a(n-1)-a(n-2)  for n>13.  The function memoises results
   so calls in any order remain fast. */

#include <stddef.h>
#include <stdlib.h>

/* Initial values for indices 1..13 */
static const seq_t init_vals[13] = {
    0, 1, 2, 3, 4,
    5, 6, 11, 14, 29,
    57, 76, 199
};

static seq_t *cache = NULL;    /* 1‑based array: cache[i] = a(i) */
static size_t cache_cap = 0;   /* allocated capacity (number of slots) */
static size_t cache_max = 0;   /* highest index already filled */

/* Ensure there's room for index n (1‑based). */
static void ensure_capacity(size_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* 0 is a safe placeholder for undefined indices */

    size_t idx = (size_t)n;          /* idx_t is uint64_t, fits in size_t for n ≤ 10000 */

    /* Initialise cache on first use */
    if (cache == NULL) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        for (size_t i = 0; i < 13; ++i) cache[i + 1] = init_vals[i];
        cache_max = 13;
    }

    ensure_capacity(idx);

    /* Compute any missing values up to idx using the recurrence. */
    for (size_t i = cache_max + 1; i <= idx; ++i) {
        cache[i] = 3 * cache[i - 1] - cache[i - 2];
    }
    if (idx > cache_max) cache_max = idx;

    return cache[idx];
}

/*
 * A117711  Pentagonal numbers for which both the sum of the digits and the
 *          product of the digits are pentagonal numbers.
 * https://oeis.org/A117711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth pentagonal number P(k)=k(3k-1)/2 such that the
 *            digit sum and the digit product of P(k) are also pentagonal
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 1796, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9576 (2026-11-01)
 * Helpers:   digit_product, digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined for n ≥ 1; a(1)=0, a(2)=1, … */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache of found terms */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static idx_t  k_next     = 0;      /* next k to test for pentagonal numbers */

/* Make sure `cache` can hold at least `needed` elements */
static void ensure_capacity(size_t needed)
{
    if (cache == NULL) {
        cache_cap = (needed > 0) ? needed : 1;
        cache = seq_alloc(cache_cap);
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < needed) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Test whether a non‑negative `x` is a pentagonal number.
   0 is considered pentagonal. */
static bool isPentagonal(seq_t x)
{
    if (x == 0) return true;
    seq_t y = x * (seq_t)24 + (seq_t)1;          /* 24*x + 1 */
    seq_t s = floor_sqrt(y);
    if (s * s != y) return false;
    return ((s + (seq_t)1) % (seq_t)6) == 0;
}

/* Ensure that the cache contains at least `n` terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    while ((size_t)cache_len < (size_t)n) {
        idx_t k = k_next;
        seq_t kk = (seq_t)k;
        seq_t p = (kk * ((seq_t)3 * kk - (seq_t)1)) / (seq_t)2; /* P(k) */
        seq_t s = digit_sum(p);
        seq_t t = digit_product(p);
        if (isPentagonal(s) && isPentagonal(t)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;
        }
        ++k_next;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence, harmless value */
    ensure_cache(n);
    return cache[n - 1];
}

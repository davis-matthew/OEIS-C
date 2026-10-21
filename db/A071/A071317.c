/*
 * A071317  a(n) = a(n-1) + sum of digits of n^2.
 * https://oeis.org/A071317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + sum of digits of n^2, a(0)=0
 * Range:     u8 n <= 25, u16 n <= 2492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* sum of decimal digits of a 64‑bit unsigned integer */
static seq_t digit_sum_uint64(uint64_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* sum of digits of i² (i fits in idx_t, i² fits in 64 bits for the
   intended range; use 128‑bit intermediate if available) */
static seq_t digit_sum_square(idx_t i)
{
    unsigned __int128 sq128 = (unsigned __int128)i * i;
    uint64_t sq64 = (uint64_t)sq128;        /* works for i^2 < 2^64 */
    return digit_sum_uint64(sq64);
}

/* Cache for computed values */
static seq_t *cache = NULL;
static idx_t   cache_cap   = 0;   /* allocated size */
static idx_t   cache_used  = 0;   /* elements initialized */
static idx_t   last_computed = 0; /* highest n with a(n) computed */

/* ensure the cache has at least (needed + 1) elements */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *new_p = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_p) return;        /* out‑of‑memory: silently fail, caller will crash */

    if (cache) {
        for (idx_t i = 0; i < cache_used; ++i) new_p[i] = cache[i];
        free(cache);
    }
    cache = new_p;
    cache_cap = new_cap;
    /* initialise new slots to zero (good for arbitrary‑precision types) */
    for (idx_t i = cache_used; i < cache_cap; ++i) cache[i] = 0;
    cache_used = cache_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!cache) {                 /* first call */
        ensure_cache(1);
        cache[0] = 0;
        last_computed = 0;
    }

    if (n >= cache_used)    /* enlarge array if needed */
        ensure_cache(n);

    while (last_computed < (idx_t)n) {
        idx_t    i   = last_computed + 1;
        seq_t    prev  = cache[last_computed];
        seq_t    add   = digit_sum_square(i);
        cache[i] = prev + add;
        last_computed = i;
    }

    return cache[n];
}

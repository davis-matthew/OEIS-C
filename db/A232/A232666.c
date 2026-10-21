/*
 * A232666  6-free Fibonacci numbers.
 * https://oeis.org/A232666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n≥2 a(n) = (a(n−1)+a(n−2)) / 6^k where k is
 *            the largest integer such that 6^k divides a(n−1)+a(n−2)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values */
static seq_t *cache   = NULL;   /* pointer to allocated array */
static size_t cache_cap = 0;    /* allocated capacity in elements */
static idx_t  cache_size = 0;   /* number of already computed terms (next index to fill) */

/* Ensure the cache can hold at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
    if (!cache) abort();   /* allocation failure */
}

/* 6‑free Fibonacci number a(n) */
seq_t a(idx_t n)
{
    if (n < cache_size) return cache[n];  /* already computed */

    ensure_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t sum = cache[i-1] + cache[i-2];
            /* divide by the largest power of 6 */
            while ((sum % 6) == 0) {
                sum /= 6;
            }
            cache[i] = sum;
        }
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A232601  a(n) = Sum_{k=0..n} k^p*q^k for p = 2 and q = -2.
 * https://oeis.org/A232601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*((-2)^n * (9*n^2 + 6*n - 1) + 1)/27 Recurrence: a(n) =
 *            -5*a(n-1) - 6*a(n-2) + 4*a(n-3) + 8*a(n-4) Initial values:
 *            a(0)=0, a(1)=-2, a(2)=14, a(3)=-58
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 22, i64 n <= 52, i128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* dynamic array of already computed terms */
static idx_t   cache_len = 0; /* number of valid terms in cache (0..cache_len-1) */
static idx_t   cache_cap = 0; /* allocated capacity of the cache */

static void ensure_capacity(idx_t cap)
{
    if (cap <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < cap) new_cap = cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache_len > 0) return;
    ensure_capacity(4);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)(-2);
    cache[2] = (seq_t)14;
    cache[3] = (seq_t)(-58);
    cache_len = 4;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_len) return cache[n];

    /* extend the cache up to index n */
    ensure_capacity(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (sizeof(seq_t) <= 8) {
            /* use 128‑bit intermediates to avoid signed overflow */
            __int128 t = 0;
            t += (__int128)cache[i-1] * (-5);
            t += (__int128)cache[i-2] * (-6);
            t += (__int128)cache[i-3] * 4;
            t += (__int128)cache[i-4] * 8;
            val = (seq_t)t;
        } else {
            val = -5 * cache[i-1] - 6 * cache[i-2] + 4 * cache[i-3] + 8 * cache[i-4];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

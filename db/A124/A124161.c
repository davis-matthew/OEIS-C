/*
 * A124161  a(n) = n*(n-1)*(n^3 + 21*n^2 - 4*n + 96)/120.
 * https://oeis.org/A124161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^3 + 21*n^2 - 4*n + 96)/120 Recurrence for n >
 *            5: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) +
 *            6*a(n-5) - a(n-6) Initial values: a(0)=0, a(1)=0, a(2)=3,
 *            a(3)=15, a(4)=48, a(5)=121
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 216, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/alloc_seq.h"

static seq_t *cache = NULL;      /* dynamically grown cache */
static size_t cache_len = 0;     /* number of elements cached (>=1 means cache[0] exists) */

/* Ensure cache has space for indices 0..n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = alloc_seq(new_len);
    } else {
        /* reallocate with standard malloc semantics (works for C/C++) */
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (new_cache == NULL) abort();   /* allocation failed */
    /* initialize new entries if expanding beyond initial values */
    for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_len = new_len;
}

/* Compute a(n) using recurrence + cache */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len && cache[n] != 0) return cache[n];
    /* Set known initial values */
    if (n >= 0) cache[0] = 0;
    if (n >= 1) cache[1] = 0;
    if (n >= 2) cache[2] = 3;
    if (n >= 3) cache[3] = 15;
    if (n >= 4) cache[4] = 48;
    if (n >= 5) cache[5] = 121;
    /* Compute remaining terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        /* Use a signed wide type for intermediate to avoid unsigned wrap‑around */
        int64_t t = (int64_t)(6)   * (int64_t)cache[i-1]
                  - (int64_t)(15)  * (int64_t)cache[i-2]
                  + (int64_t)(20)  * (int64_t)cache[i-3]
                  - (int64_t)(15)  * (int64_t)cache[i-4]
                  + (int64_t)(6)   * (int64_t)cache[i-5]
                  - (int64_t)      (int64_t)cache[i-6];
        cache[i] = (seq_t)t;
    }
    return cache[n];
}

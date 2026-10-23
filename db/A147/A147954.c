/*
 * A147954  a(0) = 0, a(1) = a(2) = 1, a(n) = a(a(n-1)) + a(n-a(n-1)) for 3 <=
 *          n <= 5, and a(n) = a(a(n-1)) + r(n) for n >= 6, where r(n) =
 *          a(a(floor(n/6))) for n == 0, 1, 2, 3, 4 (mod 6), and r(n) = a(n -
 *          a(floor(n/6))) for n == 5 (mod 6).
 * https://oeis.org/A147954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=a(2)=1, a(n)=a(a(n-1))+a(n-a(n-1)) for 3<=n<=5,
 *            a(n)=a(a(n-1))+r(n) for n>=6, r(n)=a(a(floor(n/6))) if n%6<5,
 *            r(n)=a(n-a(floor(n/6))) if n%6==5
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal static cache for memoisation. */
static seq_t *cache = NULL;   /* array of computed values */
static size_t cached_len = 0; /* number of elements in cache (next index to compute) */
static size_t cached_cap = 0; /* allocated capacity */

/* Grow the cache to at least size 'n+1'. */
static void ensure_cache(idx_t n)
{
    if (cached_len > n) return;                /* already computed */

    /* Allocate or grow the cache. */
    size_t required = (size_t)n + 1;
    if (required > cached_cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();              /* out of memory */
        cache = new_cache;
        cached_cap = new_cap;
    }

    /* Compute values up to n. */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 0;
        } else if (i == 1 || i == 2) {
            val = 1;
        } else if (i <= 5) {
            /* a(i) = a(a(i-1)) + a(i - a(i-1))  for 3<=i<=5 */
            idx_t j = (idx_t)cache[i - 1];             /* a(i-1) */
            val = cache[j] + cache[i - j];
        } else {
            /* For i >= 6: a(i) = a(a(i-1)) + r(i) */
            idx_t j = (idx_t)cache[i - 1];             /* a(i-1) */
            seq_t r;
            idx_t mod = i % 6;
            if (mod < 5) {
                idx_t ii = i / 6;                      /* floor(i/6) */
                idx_t jj = (idx_t)cache[ii];           /* a(floor(i/6)) */
                r = cache[jj];
            } else { /* mod == 5 */
                idx_t ii = i / 6;
                idx_t jj = (idx_t)cache[ii];
                r = cache[i - jj];
            }
            val = cache[j] + r;
        }
        cache[i] = val;
    }
    cached_len = n + 1;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

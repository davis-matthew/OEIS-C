/*
 * A140149  a(1)=1, a(n)=a(n-1)+n^2 if n odd, a(n)=a(n-1)+ n^3 if n is even.
 * https://oeis.org/A140149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 429, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=a(n-1)+n^2 if n is odd, a(n)=a(n-1)+n^3 if n is even. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_len = 0;   /* number of terms stored (up to cache_len) */
static idx_t cache_cap = 0;   /* current capacity of cache array */

static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 1 : (size_t)cache_cap;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failed */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; n=0 not used */

    ensure_capacity(n);

    if (cache_len == 0) {
        cache[0] = (seq_t)1; /* a(1) */
        cache_len = 1;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t nn = (seq_t)i;
        seq_t add = (i & 1ULL) ? nn * nn : nn * nn * nn;
        cache[i - 1] = cache[i - 2] + add;
    }

    cache_len = n;
    return cache[n - 1];
}

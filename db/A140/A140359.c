/*
 * A140359  a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3), with initial terms 1, 1, 6.
 * https://oeis.org/A140359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3); a(0)=1, a(1)=1, a(2)=6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – terminate */
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Convert to a type suitable for array indices */
    size_t N = (size_t)n;

    /* Initialize cache on first use */
    if (cache_len == 0) {
        ensure_cache_size(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* Return early if already computed */
    if (N < cache_len)
        return cache[N];

    /* Compute values up to N and store them */
    for (size_t i = cache_len; i <= N; ++i) {
        seq_t t1 = cache[i-1] + cache[i-1];      /* 2 * a(i-1) */
        seq_t t2 = cache[i-2];                   /* a(i-2) */
        seq_t t3 = cache[i-3] + cache[i-3];      /* 2 * a(i-3) */
        seq_t val = t1 + t2 - t3;                /* recurrence */
        ensure_cache_size(i + 1);
        cache[i] = val;
    }
    cache_len = N + 1;
    return cache[N];
}

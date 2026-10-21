/*
 * A094833  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 3, s(2n) = 5.
 * https://oeis.org/A094833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6a(n-1) - 9a(n-2) + a(n-3) initial values: a(1)=1,
 *            a(2)=4, a(3)=15
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10001] = {0};  /* cache[0] unused, cache[1..10000] */
static size_t cache_len = 0;      /* highest index stored in cache */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset not defined below 1 */

    const size_t N = (size_t)n;

    /* handle small n directly */
    if (N <= 3) {
        switch (N) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)4;
            case 3: return (seq_t)15;
        }
    }

    /* if already computed, return cached value */
    if (cache_len >= N) {
        return cache[N];
    }

    /* compute up to min(N, 10000) using cache */
    size_t target = N;
    if (target > 10000) target = 10000;

    if (cache_len == 0) {                /* first time: initialise base terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        cache_len = 3;
    }

    for (size_t i = cache_len + 1; i <= target; ++i) {
        cache[i] = 6 * cache[i-1] - 9 * cache[i-2] + cache[i-3];
    }
    cache_len = target;

    if (N <= target) {
        return cache[N];
    }

    /* N > 10000: compute remaining values using a small rolling buffer */
    seq_t a0 = cache[cache_len-2];   /* a(cache_len-2) */
    seq_t a1 = cache[cache_len-1];   /* a(cache_len-1) */
    seq_t a2 = cache[cache_len];     /* a(cache_len) */
    seq_t next;

    for (size_t i = cache_len + 1; i <= N; ++i) {
        next = 6 * a2 - 9 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}

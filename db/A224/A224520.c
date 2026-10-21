/*
 * A224520  Numbers a(n) with property a(n) + a(n+4) = 2^(n+4) - 1 =
 *          A000225(n+4).
 * https://oeis.org/A224520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + 3*a(n-5) - 2*a(n-6) (for n
 *            ≥ 6). Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=7, a(4)=15,
 *            a(5)=30
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;

    /* Allocate initial cache with the first six terms. */
    if (cap == 0) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;               /* out‑of‑memory: return 0 */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 7;
        cache[4] = 15;
        cache[5] = 30;
    }

    /* Return a cached value if available. */
    if ((size_t)n < cap)
        return cache[n];

    /* Extend the cache to size n+1. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* out‑of‑memory: return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms sequentially. */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t t1 = 3 * cache[i-1];
        seq_t t2 = 3 * cache[i-5];
        seq_t sub = 2 * cache[i-2] + cache[i-4] + 2 * cache[i-6];
        cache[i] = t1 + t2 - sub;
    }

    return cache[n];
}

/*
 * A205188  Number of (n+1) X 4 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-4) for n>5; Initial values: a(1)=48,
 *            a(2)=64, a(3)=124, a(4)=160, a(5)=292
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 42, u64 n <= 92, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;          /* number of stored elements */

    /* initialise cache with the first five terms (a(1) … a(5)) */
    if (cached == 0) {
        cached = 5;
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failed – return 0 */
        cache[0] = (seq_t)48;   /* a(1) */
        cache[1] = (seq_t)64;   /* a(2) */
        cache[2] = (seq_t)124;  /* a(3) */
        cache[3] = (seq_t)160;  /* a(4) */
        cache[4] = (seq_t)292;  /* a(5) */
    }

    /* if the requested term is already cached, return it immediately */
    if ((size_t)n <= cached) {
        return cache[n - 1];
    }

    /* enlarge the cache to hold up to the requested index */
    size_t old_cap = cached;
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;                      /* allocation failed – return 0 */
    cache = tmp;
    cached = new_cap;

    /* compute any newly required terms using the recurrence */
    for (size_t i = old_cap; i < new_cap; ++i) {   /* zero‑based index */
        if (i >= 5) {                      /* a(n) with n = i+1 >= 6 */
            cache[i] = cache[i - 2] * (seq_t)2 + cache[i - 4];
        }
    }

    return cache[n - 1];
}

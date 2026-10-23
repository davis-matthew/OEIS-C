/*
 * A195438  Six-digit exclusionary squares: 6-digit numbers whose squares
 *          comprise digits none of which appears in the number itself.
 * https://oeis.org/A195438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1219, u128 n <= 1219,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Six‑digit exclusionary squares
 * A195438 – numbers n such that 100000 ≤ n ≤ 999999 and
 *          no digit occurring in n occur in n^2.
 * There are exactly 1219 such numbers; the sequence is sorted ascending.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /*  Static cache of the 1219 numbers, generated once. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Build the cache on first call. */
    if (cache_len == 0) {
        /* There are 900 000 candidates, but we know only 1219 are valid,
           so allocate the maximum necessary for simplicity. */
        const size_t max_candidates = 900000;
        cache = (seq_t *)malloc(max_candidates * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        size_t found = 0;
        const idx_t START = 100000;
        const idx_t END   = 999999;

        for (idx_t x = START; x <= END; ++x) {
            uint64_t ux = (uint64_t)x;
            uint64_t maskX = 0;
            uint64_t tmp = ux;
            while (tmp) { maskX |= 1ULL << (tmp % 10); tmp /= 10; }

            seq_t y = (seq_t)ux * (seq_t)ux;      /* y = x^2, fits in 64 bits */
            uint64_t uy = (uint64_t)y;
            uint64_t maskY = 0;
            tmp = uy;
            while (tmp) { maskY |= 1ULL << (tmp % 10); tmp /= 10; }

            if ((maskX & maskY) == 0) {
                cache[found++] = (seq_t)x;
                if (found >= max_candidates) break; /* safety */
            }
        }
        cache_len = found;
        /* Optionally shrink allocated memory: */
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* If n is within range, return the nth term; otherwise return 0. */
    if (n == 0 || n > (idx_t)cache_len) {
        return (seq_t)0;
    }
    return cache[n - 1];
}

/*
 * A165064  Number of cycles of n-digit numbers (including fixed points) under
 *          the base-6 Kaprekar map A165051.
 * https://oeis.org/A165064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = -∑_{k=1}^{15}
 *            Q[k]·a(n‑k) + R(n) where Q = [1, 1, -1, -2, -2, -1, 1, 3, 3, 1,
 *            -1, -2, -2, -1, 1, 1] and R(n) = [0, 1, 1, 0, 0, 0, 2, 0, -2,
 *            -3, -3, -1, 2, 4, 4, 4, 1, -3, -3, -2, -1, 0, 1, 1] with a(1)=1,
 *            a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=4, a(7)=1,...
 * Range:     u8 n <= 45, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const int Q[16] = { 0, 1, -1, -2, -2, -1, 1, 3, 3, 1, -1, -2, -2, -1, 1, 1 };
static const long long R[24] = { 0, 1, 1, 0, 0, 0, 2, 0, -2, -3, -3, -1, 2, 4, 4, 4,
                                 1, -3, -3, -2, -1, 0, 1, 1 };
static const seq_t INIT[16] = { 0, 1, 0, 1, 1, 2, 4, 1, 5, 2, 7, 3, 9, 4, 13, 7 };

seq_t a(idx_t n)
{
    if (n <= 15)                  /* use predefined values */
        return INIT[(size_t)n];

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (elements 0 … cache_cap-1) */
    static size_t cached_last = 0; /* largest index computed */

    size_t target = (size_t)n;

    /* allocate or enlarge cache if necessary */
    if (cache == NULL) {
        cache_cap = 16;
        if (cache_cap <= target) cache_cap = target + 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();      /* out of memory */
        for (size_t i = 0; i <= 15; ++i) cache[i] = INIT[i];
        cached_last = 15;
    } else if (cache_cap <= target) {
        size_t new_cap = cache_cap;
        while (new_cap <= target) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cached_last + 1; i <= target; ++i) {
        long long val = 0;
        if (i < 24) val = R[i];

        for (int j = 1; j <= 15; ++j) {
            long long coeff = -Q[j];
            long long term  = (long long)cache[i - j];
            val += coeff * term;
        }
        cache[i] = (seq_t)val;
    }
    cached_last = target;

    return cache[target];
}

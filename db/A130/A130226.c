/*
 * A130226  Smallest integer x satisfying the Pell equation x^2-k*y^2=-1 for
 *          the values of k given in A031396.
 * https://oeis.org/A130226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 49, u64 n <= 157,
 *            u128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130226: a(n) = smallest integer x such that ∃ y ≥ 1
   with x² – k·y² = –1, where k = A031396(n).  a(1) = 0 by definition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = (1ULL << 32);
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;          /* by definition */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_d = 1;           /* next integer d to check */

    while (cache_len < (size_t)(n - 1)) {
        uint64_t d = next_d++;
        if (d == 1) continue;            /* skip perfect square 1 */

        uint64_t a0 = isqrt_u64(d);
        if (a0 * a0 == d) continue;      /* d is a perfect square */

        /* continued‑fraction terms of √d (excluding the final 2a0) */
        uint64_t terms[8192];
        size_t L = 0;
        uint64_t m = 0, denom = 1, a = a0;

        terms[L++] = a0;                 /* a0 is always included */

        while (true) {
            m     = denom * a - m;
            denom = (d - m * m) / denom;
            a     = (a0 + m) / denom;
            if (a == 2 * a0) break;      /* end of the period */
            if (L >= sizeof terms / sizeof terms[0]) break; /* safety */
            terms[L++] = a;
        }

        size_t period = L;
        if ((period & 1) == 0) continue; /* even period → no -1 solution */

        /* converge to the fundamental -1 solution */
        seq_t p_prev2 = 0, p_prev1 = 1;
        seq_t q_prev2 = 1, q_prev1 = 0;
        seq_t p = 0, q = 0;

        for (size_t i = 0; i < period; ++i) {
            unsigned int ai = (unsigned int)terms[i];
            p = (seq_t)ai * p_prev1 + p_prev2;
            q = (seq_t)ai * q_prev1 + q_prev2;
            p_prev2 = p_prev1; p_prev1 = p;
            q_prev2 = q_prev1; q_prev1 = q;
        }

        /* store the minimal x (which is p) */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = p;
    }

    return cache[n - 2];  /* a(2) corresponds to cache[0] */
}

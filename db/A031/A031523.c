/*
 * A031523  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 25.
 * https://oeis.org/A031523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that the
 *            continued–fraction period of √k is even and, after deleting the
 *            last term of that period, the central term equals 25
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root (floor) for unsigned 64‑bit numbers. */
static inline uint32_t isqrt_u64(idx_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint32_t)lo;
}

/* Test whether n is a perfect square. */
static inline bool is_square(idx_t n)
{
    uint32_t a = isqrt_u64(n);
    return ((idx_t)a * (idx_t)a) == n;
}

seq_t a(idx_t n)
{
    /* Static cache of the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static idx_t next_k = 2;  /* next integer k to test (skip squares) */
    static uint32_t *terms = NULL;
    static size_t terms_cap = 0;

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache if necessary. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }

    /* Main loop: find k values until we have n terms. */
    while (cache_len < n) {
        /* Skip perfect squares. */
        if (is_square(next_k)) {
            ++next_k;
            continue;
        }

        uint32_t a0 = isqrt_u64(next_k);  /* floor(√k) */
        idx_t m = 0, d = 1, a = a0;
        size_t pos = 0;

        /* Generate the periodic part of the continued fraction. */
        for (;;) {
            m = d * a - m;
            d = (next_k - m * m) / d;
            a = (a0 + m) / d;

            /* Store a. */
            if (pos == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 64;
                uint32_t *p = (uint32_t *)realloc(terms, new_cap * sizeof(uint32_t));
                terms = p;
                terms_cap = new_cap;
            }
            terms[pos++] = (uint32_t)a;

            if (a == 2 * a0)  /* last term reached */
                break;
        }

        /* pos is the period length L (including the final 2·a0). */
        if ((pos & 1) == 0) {                  /* L even */
            uint32_t central = terms[(pos >> 1) - 1];
            if (central == 25) {
                cache[cache_len++] = (seq_t)next_k;
            }
        }

        ++next_k;
    }

    return cache[n - 1];
}

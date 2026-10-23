/*
 * A031812  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 44 ones.
 * https://oeis.org/A031812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A031812: numbers k such that the period of the continued 
 * fraction expansion of sqrt(k) contains exactly 44 ones.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Fast integer square‑root for 64‑bit unsigned */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Ensure that 'results' can hold at least 'need' elements */
static void ensure_capacity(size_t *cap, seq_t **ptr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        /* if realloc fails, we terminate silently */
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *results = NULL;   /* cached found k's */
    static size_t capacity = 0;     /* allocated size */
    static size_t found = 0;        /* how many values have been found */
    static uint64_t last_checked = 0; /* last k examined */

    /* Find additional k's until we have at least n of them */
    while (found < n) {
        uint64_t k = ++last_checked;
        uint64_t a0 = floor_sqrt_u64(k);
        if (a0 * a0 == k) /* skip perfect squares */
            continue;

        uint64_t m = 0, d = 1, a = a0;
        uint64_t ones = 0;

        for (;;) {
            m = d * a - m;
            /* use 128‑bit intermediate to avoid overflow */
            d = (k - (unsigned __int128)m * m) / d;
            a = (a0 + m) / d;
            if (a == 2 * a0)        /* period finished */
                break;
            if (a == 1)
                ++ones;
        }

        if (ones == 44) {
            /* store the found value */
            ensure_capacity(&capacity, &results, found + 1);
            results[found++] = (seq_t)k;
        }
    }

    return results[n - 1];
}

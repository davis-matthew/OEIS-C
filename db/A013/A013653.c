/*
 * A013653  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly two 1's and they are not consecutive.
 * https://oeis.org/A013653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 4973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013653: numbers k such that the continued‑fraction period of √k
   contains exactly two 1's and the 1's are not consecutive.
   The function computes the n‑th term by incremental search and memoises
   previously found terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root: floor(√x) for 0 ≤ x < 2^64 . */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;   /* highest power of 4 ≤ x   */
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Test whether k satisfies the continued‑fraction property. */
static bool has_two_nonconsecutive_ones(uint64_t k)
{
    uint64_t a0 = isqrt_uint64(k);
    if (a0 * a0 == k)        /* skip perfect squares */
        return false;

    uint64_t m = 0, d = 1, a = a0;
    int ones = 0;
    bool prev_one = false;

    while (1) {
        /* m = d*a - m  (use 128‑bit intermediate to avoid overflow) */
        unsigned __int128 tmp = (unsigned __int128)d * a;
        m = (uint64_t)(tmp - m);

        /* d = (k - m²) / d */
        unsigned __int128 tmp2 = (unsigned __int128)k -
                                 (unsigned __int128)m * m;
        d = (uint64_t)(tmp2 / d);

        a = (a0 + m) / d;

        if (a == 1) {
            if (prev_one)            /* consecutive 1's → reject   */
                return false;
            ones++;
            if (ones > 2)            /* more than two 1's */
                return false;
            prev_one = true;
        } else {
            prev_one = false;
        }

        if (a == 2 * a0)            /* end of period reached   */
            break;
    }
    return ones == 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 2;   /* integers to test */

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n) {
        if (has_two_nonconsecutive_ones(next_k)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}

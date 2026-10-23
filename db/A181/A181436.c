/*
 * A181436  Numbers k such that the prime divisors of k^2 + 1 are of the form
 *          q^2 + 1.
 * https://oeis.org/A181436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that every prime divisor of k^2+1 is of the form q^2+1.
   The function returns the n‑th such number (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* ------------------------------------------------------------------ */
/* Integer square root for uint64_t (floor)                            */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;            /* highest power of 4 ≤ 2^64      */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;                                /* r*r ≤ original n < (r+1)^2 */
}

/* ------------------------------------------------------------------ */
/* Test whether p = q² + 1 for some integer q.                        */
/* ------------------------------------------------------------------ */
static bool is_special_prime(uint64_t p)
{
    if (p < 2)          return false;
    if (p == 2)         return true;          /* 2 = 1² + 1 */
    uint64_t t = isqrt_u64(p - 1);
    return t * t == p - 1;
}

/* ------------------------------------------------------------------ */
/* Compute the n‑th term of the sequence (1‑based index).              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 1;          /* next integer to test */

    while (cache_len < (size_t)n) {
        uint64_t val = next_k * next_k + 1;
        uint64_t primes[64], exps[64];   /* large enough for any 64‑bit value */
        size_t cnt = 0;

        factor_u64(val, primes, exps, &cnt);

        bool good = true;
        for (size_t i = 0; i < cnt; ++i) {
            if (!is_special_prime(primes[i])) {
                good = false;
                break;
            }
        }

        if (good) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}

/*
 * A227815  Composite numbers n divisible by their concatenated exponents in
 *          prime factorization.
 * https://oeis.org/A227815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227815 – Composite numbers divisible by the concatenation of the
 * exponents in their prime factorization.
 *
 *  a(n) is the nth composite integer m such that
 *       m % concat_exponents(m) == 0
 *  where concat_exponents(m) is the decimal concatenation of all
 *  exponents in the prime factorisation of m.
 *
 *  Implementation: a dynamic incremental search with memoisation.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

/* Count decimal digits of a non‑zero 64‑bit integer. */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* Compute 10^d as a seq_t value. */
static inline seq_t pow10_uint64(int d)
{
    seq_t r = 1;
    while (d--) r *= (seq_t)10;
    return r;
}

/* Test whether a candidate m satisfies the property. */
static bool is_good(uint64_t m)
{
    if (m <= 3) return false;                /* 1,2,3 are not composite */

    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(m, primes, exps, &cnt);

    /* Prime test – a single prime factor with exponent 1. */
    if (cnt == 1 && exps[0] == 1) return false;

    seq_t concat = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t exp = exps[i];
        int d = digits_uint64(exp);
        seq_t mult = pow10_uint64(d);       /* 10^d */
        concat = concat * mult + (seq_t)exp;
    }

    if (concat == 0) return false;           /* safety check */
    return (((seq_t)m) % concat) == 0;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but safe */

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 3;         /* we start testing from 4 */

    /* Return cached term if available. */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* Allocate cache on first use. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Continue searching until we have the desired term. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_good(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n-1];
}

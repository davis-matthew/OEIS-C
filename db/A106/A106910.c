/*
 * A106910  Primes of the form 2x^2+xy+7y^2, with x and y nonnegative.
 * https://oeis.org/A106910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes of the form 2x^2 + x*y + 7y^2 with x,y ≥ 0 (OEIS
 *            A106910). The function generates the primes in increasing order
 *            and memoises them for fast repeated access. Representation is
 *            tested by solving 8(p-7y^2) = s^2 - y^2, which is equivalent to
 *            the given quadratic form
 * Range:     u8 n <= 7, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_uint64.h"   /* for is_prime_uint64 */

/* integer square root of a 128‑bit integer, result fits in 64 bits */
static uint64_t isqrt_uint128(__uint128_t n)
{
    uint64_t low = 0;
    /* upper bound: sqrt(8 * 2^63) < 2^35-1 */
    uint64_t high = (uint64_t)1 << 35;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* !! static storage for memoisation of the sequence values !! */
static seq_t *vals   = NULL;
static size_t val_sz = 0;
static size_t val_cap = 0;
static uint64_t last_checked = 1;    /* last prime examined */

/* helper: build primes of the required form until we have at least n terms */
static void ensure_n(idx_t n)
{
    while (val_sz < n) {
        /* find next prime */
        do {
            ++last_checked;
        } while (!is_prime_uint64(last_checked));
        uint64_t p = last_checked;

        /* check representation 2x^2 + xy + 7y^2 = p */
        bool ok = false;
        for (uint64_t y = 0;; ++y) {
            __uint128_t yy128 = (__uint128_t)y * y;
            __uint128_t prod  = (__uint128_t)7 * yy128;
            if (prod > p)
                break;
            uint64_t rem = p - (uint64_t)prod;          /* p - 7y^2 */
            __uint128_t D128 = yy128 + (__uint128_t)8 * rem; /* y^2 + 8*rem */
            uint64_t s = isqrt_uint128(D128);
            if ((__uint128_t)s * s == D128) {
                if (s >= y && ((s - y) & 3ULL) == 0) {
                    ok = true;
                    break;
                }
            }
        }

        if (ok) {
            if (val_sz == val_cap) {
                val_cap = val_cap ? val_cap * 2 : 64;
                vals = (seq_t *)realloc(vals, val_cap * sizeof(seq_t));
            }
            vals[val_sz++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, behaviour for n<1 irrelevant */
        return 0;
    ensure_n(n);
    return vals[n - 1];
}

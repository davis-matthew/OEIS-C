/*
 * A199992  Composite numbers whose multiplicative persistence is 2.
 * https://oeis.org/A199992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 969 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199992: Composite numbers whose multiplicative persistence is 2.
   For each n ≥ 1, a(n) is the n‑th composite integer with persistence exactly 2. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* True iff x > 1 and not prime, i.e. x is composite. */
static bool is_composite(seq_t x)
{
    if (x <= 3) {
        return false;                 /* 1, 2, 3 are not composite */
    }
    /* For larger numbers, rely on the prime test. */
    return !is_prime_big(x);
}

/* True iff the multiplicative persistence of x is exactly 2. */
static bool has_persistence_two(seq_t x)
{
    seq_t p1 = digit_product(x);
    if (p1 < 10) {
        return false;                 /* persistence 1 */
    }
    seq_t p2 = digit_product(p1);
    return p2 < 10;                   /* persistence 2 */
}

/* Main function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t  cached_len = 0;
    static size_t  cached_cap = 0;

    /* Current integer to test (type wide enough to hold arbitrary candidates). */
    static uint64_t current = 2;

    /* Initialise cache on first call. */
    if (cached_len == 0) {
        cached_cap = 64;
        cache = mem_new(seq_t, cached_cap);
    }

    /* Generate terms until we have at least n terms. */
    while (cached_len < (size_t)n) {
        seq_t candidate = (seq_t)current;   /* value to store in cache */

        if (is_composite(candidate) && has_persistence_two(candidate)) {
            /* Grow cache if necessary. */
            if (cached_len == cached_cap) {
                cached_cap <<= 1;
                cache = mem_resize(seq_t, cache, cached_cap);
            }
            cache[cached_len++] = candidate;
        }

        ++current;
    }

    return cache[n - 1];
}

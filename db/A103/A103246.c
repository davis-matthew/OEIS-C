/*
 * A103246  Numbers y, without duplication, in Pythagorean triples x,y,z where
 *          x,y,z are relatively prime composite numbers.
 * https://oeis.org/A103246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th smallest odd integer y that appears as the odd
 *            leg of a primitive Pythagorean triple (x,y,z) with x even, y
 *            odd, z odd where all three sides are composite and pairwise
 *            coprime. The terms are listed in increasing order of y
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9025 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if n is composite (not prime and >1) */
static inline bool isComposite(uint64_t n)
{
    if (n < 4)          /* 1,2,3 are not composite */
        return false;
    if ((n & 1ULL) == 0)/* even >2 is composite */
        return true;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return true;
    }
    return false;
}

/* Euclid's algorithm for gcd on idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache of computed terms */
static seq_t *cache = NULL;
static idx_t cached = 0;
static idx_t capacity = 0;
static idx_t nextOdd = 3;   /* next odd y to test */

/* Ensure capacity of cache array */
static void ensure_capacity(idx_t min_cap)
{
    if (capacity >= min_cap)
        return;
    while (capacity < min_cap)
        capacity = (capacity == 0) ? 64 : capacity << 1;
    seq_t *tmp = mem_resize(seq_t, cache, (size_t)capacity);
    if (!tmp && capacity > 0) {
        mem_free(cache);
        cache = NULL;
        capacity = 0;
    }
    cache = tmp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Return cached value if already computed */
    if (cached >= n)
        return cache[n - 1];

    /* Generate terms until we have at least n */
    for (;;) {
        /* Skip any already generated y's by incrementing nextOdd */
        if (cached >= n)
            break;
        idx_t y = nextOdd;          /* candidate odd leg */
        nextOdd += 2;               /* prepare for next iteration */

        bool found = false;
        /* Loop over odd divisors d of y */
        for (idx_t d = 1; d <= y / d; d += 2) {
            if (y % d != 0)
                continue;
            idx_t d2 = y / d;
            if (d >= d2)            /* ensure d < d2 to avoid duplicate pairs */
                break;
            idx_t m = (d + d2) >> 1;
            idx_t k = (d2 - d) >> 1;
            if (gcd_idx(m, k) != 1)
                continue;              /* not coprime -> not primitive */

            /* Compute the sides */
            uint64_t a_val = (uint64_t)2 * m * k;                 /* 2mk */
            uint64_t c_val = (uint64_t)m * m + (uint64_t)k * k;   /* m^2 + k^2 */

            if (isComposite(a_val) && isComposite((uint64_t)y) && isComposite(c_val)) {
                found = true;
                break;
            }
        }

        if (found) {
            ensure_capacity(cached + 1);
            cache[cached++] = y;
            if (cached >= n)
                break;
        }
    }

    return cache[n - 1];
}

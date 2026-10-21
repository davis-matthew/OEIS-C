/*
 * A227001  Let x0 x1 x2...xq denote the decimal expansion of n. Sequence
 *          lists numbers n such that x0/x1 + x1/x2 + ... + xq/x0 is an
 *          integer.
 * https://oeis.org/A227001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227001: a(n) is the nth integer whose decimal digits are
   all non‑zero and for which ∑ d_i/d_{i+1} (with cyclic indices) is an integer.
   The function enumerates all candidates in increasing order and caches
   the results.  It works for any seq_t type that fits the listed
   constraints. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Euclidean GCD for 32‑bit unsigned integers. */
static inline uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether a positive integer has the required property.
   The function uses only 32‑bit arithmetic; the intermediate values
   are all < 2^32, so it works even when seq_t is 8 or 16 bits. */
static bool good_number(uint64_t num)
{
    /* Gather the digits in reverse order into a small buffer. */
    uint8_t rev[20];
    int len = 0;
    while (num > 0) {
        uint8_t d = (uint8_t)(num % 10);
        if (d == 0)            /* zero digit makes the sum undefined */
            return false;
        rev[len++] = d;
        num /= 10;
    }
    if (len == 0)              /* num == 0 (should not happen) */
        return false;

    /* Compute the least common multiple of all digits. */
    uint32_t l = 1;
    for (int i = len - 1; i >= 0; --i) {
        uint32_t d = rev[i];
        uint32_t g = gcd32(l, d);
        l = (l / g) * d;      /* l ≤ 2520, so fits in 32 bits */
    }

    /* Evaluate the numerator (sum of d_i * (l / d_{i+1})). */
    uint32_t sum = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint32_t di = rev[i];
        uint32_t dine = rev[(i - 1 + len) % len];
        sum += di * (l / dine);
    }

    return (sum % l) == 0;
}

/* Static cache of computed terms. */
static uint64_t          next_candidate = 1;   /* next number to test   */
static seq_t            *cache          = NULL; /* array of a(i) values */
static size_t            cached_len     = 0;    /* number of terms stored */
static size_t            cached_cap     = 0;    /* current capacity */

/* Main sequence function.  It can be called with n in any order. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    while (cached_len < n) {
        if (good_number(next_candidate)) {
            /* Grow the cache if necessary. */
            if (cached_len == cached_cap) {
                size_t new_cap = (cached_cap == 0) ? 16 : cached_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

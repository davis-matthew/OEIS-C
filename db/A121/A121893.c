/*
 * A121893  Composite integers not equal to k*[k or k+1 or k+2] where k is a
 *          natural number.
 * https://oeis.org/A121893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9877 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A121893: composite integers not equal to k·k, k·(k+1), or k·(k+2) 
 * (for any natural number k ≥ 1). 
 * a(1) = 10, a(2) = 14, …
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Test whether a composite number x (x ≥ 4) has one of the forbidden forms.  */
static bool is_forbidden(idx_t x) {
    /* 1. k*k  */
    idx_t k = isqrt(x);
    if (k * k == x) return true;

    /* 2. k*(k+1)  → 4x+1 must be a perfect square */
    idx_t t = 4 * x + 1;
    idx_t sqrt_t = isqrt(t);
    if (sqrt_t * sqrt_t == t) {
        if ((sqrt_t - 1) % 2 == 0) {
            idx_t k2 = (sqrt_t - 1) / 2;
            if (k2 >= 1 && k2 * (k2 + 1) == x) return true;
        }
    }

    /* 3. k*(k+2)  →  x+1 must be a perfect square */
    idx_t y = x + 1;
    idx_t sqrt_y = isqrt(y);
    if (sqrt_y * sqrt_y == y) {
        if (sqrt_y >= 2) {
            idx_t k3 = sqrt_y - 1;
            if (k3 * (k3 + 2) == x) return true;
        }
    }

    return false;
}

/* The required sequence function.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for the OEIS offset 1   */

    idx_t count = 0;
    idx_t cur   = 4;                /* first composite number */

    for (;;) {
        /* Skip primes. */
        if (!isPrimeGeneric((seq_t)cur)) {
            /* cur is composite: check forbidden forms. */
            if (!is_forbidden(cur)) {
                ++count;
                if (count == n) return (seq_t)cur;
            }
        }
        ++cur;
    }
}

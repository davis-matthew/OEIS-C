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
 * Helpers:   _floor_isqrt_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A121893: composite integers not equal to k·k, k·(k+1), or k·(k+2) 
 * (for any natural number k ≥ 1). 
 * a(1) = 10, a(2) = 14, …
 */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"      /* isPrimeGeneric(seq_t)           */
#include "../utils/_floor_isqrt_u64.h"    /* _floor_isqrt_u64(idx_t)        */

/* Test whether a composite number x (x ≥ 4) has one of the forbidden forms.  */
static bool is_forbidden(idx_t x) {
    /* 1. k*k  */
    idx_t k = _floor_isqrt_u64(x);
    if (k * k == x) return true;

    /* 2. k*(k+1)  → 4x+1 must be a perfect square */
    idx_t t = 4 * x + 1;
    idx_t sqrt_t = _floor_isqrt_u64(t);
    if (sqrt_t * sqrt_t == t) {
        if ((sqrt_t - 1) % 2 == 0) {
            idx_t k2 = (sqrt_t - 1) / 2;
            if (k2 >= 1 && k2 * (k2 + 1) == x) return true;
        }
    }

    /* 3. k*(k+2)  →  x+1 must be a perfect square */
    idx_t y = x + 1;
    idx_t sqrt_y = _floor_isqrt_u64(y);
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

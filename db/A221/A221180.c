/*
 * A221180  Erroneous version of A000079.
 * https://oeis.org/A221180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^n (for n < 13) * a(n) = 2^n - 100 * 2^(n-13) (for n >=
 *            13)
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Compute 2^n using the provided utility and return it directly
       for the small indices where the sequence coincides with powers
       of two. */
    if (n < 13U) {
        return pow2(n);
    }

    /* For n >= 13 we need to subtract 100 * 2^(n-13). */
    seq_t pow_n   = pow2(n);           /* 2^n  */
    seq_t pow_n13 = pow2(n - 13U);     /* 2^(n-13) */
    seq_t sub     = mul_small(pow_n13, 100U); /* 100 * 2^(n-13) */

    seq_t res = pow_n;
    res -= sub;
    return res;
}

/*
 * A086459  Determinant of the circulant matrix whose rows are formed by
 *          successively rotating the vector (1, 2, 4, 8, ..., 2^(n-1)) right.
 * https://oeis.org/A086459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (1-2^n)^(n-1) for n ≥ 1, a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) */
        return (seq_t)1;

    seq_t pow2n = pow2(n);           /* 2^n  (pow2 expects idx_t) */
    seq_t base   = (seq_t)1 - pow2n; /* 1‑2^n */
    return ipow(base, (seq_t)(n - 1));
}

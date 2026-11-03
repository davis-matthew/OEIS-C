/*
 * A091103  Expansion of (1-3*x+12*x^2)/((1-3*x)*(1+3*x)).
 * https://oeis.org/A091103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1 for n ≥ 1, if n is odd: a(n) = –3ⁿ if n is even: a(n) =
 *            7 · 3ⁿ⁻¹ (derived from (1‑3x+12x²)/((1‑3x)(1+3x)).)
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* power of 3, exponentiation by squaring */
static seq_t pow3_uint64(uint64_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    if (n & 1U)  /* odd */
        return -(pow3_uint64(n));
    else         /* even */
        return (seq_t)7 * pow3_uint64(n - 1U);
}

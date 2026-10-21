/*
 * A227621  The nearest integer of perimeter of T-square (fractal) after
 *          n-iterations, starting with a unit square.
 * https://oeis.org/A227621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(8 * (3/2)^n) - 4 (n >= 1), a(0) = 4. This is
 *            equivalent to a(n) = round((3^n - 2^(n-1)) / 2^(n-3)) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    /* For n <= 3 we can cancel the 2^n factor before division. */
    if (n <= 3) {
        idx_t shift = 3 - n;                         /* 0 <= shift <= 3 */
        seq_t pow3 = big_pow((seq_t)3, (seq_t)n);    /* 3^n   */
        seq_t val = pow3 * ((seq_t)1 << shift);      /* 3^n * 2^(3-n) = 3^n * 2^(shift) */
        return val - ((seq_t)4);
    }

    /* n > 3: use the general rounding formula */
    seq_t pow3   = big_pow((seq_t)3, (seq_t)n);          /* 3^n   */
    seq_t denom  = big_pow((seq_t)2, (seq_t)(n - 3));    /* 2^(n-3) */
    seq_t half   = denom / ((seq_t)2);                   /* 2^(n-4) */
    seq_t r      = (pow3 + half) / denom;                /* round(3^n / 2^(n-3)) */
    return r - ((seq_t)4);
}

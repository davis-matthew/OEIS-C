/*
 * A038544  a(n) = Sum_{i=0..10^n} i^3.
 * https://oeis.org/A038544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((10^n) * (10^n + 1) / 2)^2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p10  = pow10(n);          /* 10^n */
    seq_t t1   = p10 + 1;           /* 10^n + 1 */
    seq_t t2   = p10 * t1;          /* 10^n * (10^n + 1) */
    t2 = t2 / 2;                    /* divide by 2 – exact */
    return t2 * t2;                 /* square */
}

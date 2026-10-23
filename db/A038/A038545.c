/*
 * A038545  a(n) = Sum_{i=0..10^n} i^10.
 * https://oeis.org/A038545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..10^n} i^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via an integer polynomial in t = 10^n + 1: */
/*  a(n) = (6t^11 - 33t^10 + 55t^9 - 66t^7 + 66t^5 - 33t^3 + 5·10^n + 5) / 66 */
/* where t = 10^n + 1. */

#include <stdbool.h>
#include "../utils/pow10.h"

/* Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* m = 10^n */
    seq_t m = pow10(n);
    /* t = m + 1 = 10^n + 1 */
    seq_t t = m + (seq_t)1;

    /* successive powers of t up to t^11 */
    seq_t t2 = t * t;
    seq_t t3 = t2 * t;
    seq_t t4 = t3 * t;
    seq_t t5 = t4 * t;
    seq_t t6 = t5 * t;
    seq_t t7 = t6 * t;
    seq_t t8 = t7 * t;
    seq_t t9 = t8 * t;
    seq_t t10 = t9 * t;
    seq_t t11 = t10 * t;

    /* numerator of the expression */
    seq_t num = (seq_t)6 * t11;
    num -= (seq_t)33 * t10;
    num += (seq_t)55 * t9;
    num -= (seq_t)66 * t7;
    num += (seq_t)66 * t5;
    num -= (seq_t)33 * t3;
    num += (seq_t)5 * m;
    num += (seq_t)5;

    /* final result */
    return num / (seq_t)66;
}

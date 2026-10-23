/*
 * A174680  Numerator of 1/16 - 1/n^2, using -1 at the pole where n=0.
 * https://oeis.org/A174680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = numerator of the reduced fraction (1/16 - 1/n²), with
 *            a(0) = -1. For n ≥ 1, a(n) = (n² - 16) / gcd(n² - 16, 16·n²)
 * Range:     i8 n <= 2, i16 n <= 45, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;      /* defined pole */

    seq_t nn = (seq_t)n;                /* n as seq_t */
    seq_t num = nn * nn - (seq_t)16;    /* n² - 16, may be negative */
    seq_t den = (seq_t)16 * nn * nn;    /* 16·n², always positive */

    /* Use absolute value of numerator for gcd */
    seq_t g = gcd(num < 0 ? -num : num, den);
    return num / g;
}

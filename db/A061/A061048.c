/*
 * A061048  Denominator of 1/49 - 1/n^2.
 * https://oeis.org/A061048
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the reduced form of (n^2 - 49)/(49·n^2)
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the denominator of the fraction 1/49 - 1/n². */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n < 7 the sequence is undefined; return 0 as a placeholder. */
    if (n < 7) {
        return 0;
    }

    seq_t nn = (seq_t)n * (seq_t)n;      /* n²                                */
    seq_t num = nn - (seq_t)49;          /* numerator: n² - 49                */
    seq_t den = (seq_t)49 * nn;          /* denominator: 49·n²                */

    seq_t g = gcd(num, den);             /* greatest common divisor           */
    return den / g;                      /* reduced denominator                */
}

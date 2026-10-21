/*
 * A061045  Numerator of 1/36 - 1/n^2.
 * https://oeis.org/A061045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of (1/36 - 1/n^2) in lowest terms, a(1)=−35
 * Range:     i8 n <= 1, i16 n <= 30, i32 n <= 9115, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_gcd_seq.h"

seq_t a(idx_t n) {
    /* Compute n^2.  Use seq_t to avoid overflow. */
    seq_t sq = (seq_t)n * (seq_t)n;
    /* Numerator and denominator of the fraction (n^2-36)/(36*n^2). */
    seq_t num = sq - (seq_t)36;
    if (num == 0) {
        return (seq_t)0;  /* 1/36 - 1/6^2 = 0 */
    }
    seq_t den = (seq_t)36 * sq;
    /* Reduce the fraction: divide numerator and denominator by their gcd. */
    seq_t g = _gcd_seq(num < 0 ? -num : num, den);
    return num / g;
}

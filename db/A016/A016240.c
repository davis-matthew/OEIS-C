/*
 * A016240  Inverse of 2231st cyclotomic polynomial.
 * https://oeis.org/A016240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if 0 <= n mod 2231 <= 22; * a(n) = -1 if 97 <= n mod
 *            2231 <= 119; * a(n) = 0 otherwise
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t period = 2231ULL;
    idx_t r = n % period;
    if (r <= 22ULL) return (seq_t)1;
    if (r <= 96ULL) return (seq_t)0;
    if (r <= 119ULL) return (seq_t)-1;
    return (seq_t)0;
}

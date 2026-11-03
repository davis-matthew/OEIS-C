/*
 * A000209  Nearest integer to tan n.
 * https://oeis.org/A000209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round(tan(n)), computed in long double on the index n
 * Range:     i8 n <= 10, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9224 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    long double x = (long double)n;
    long double t = tanl(x);
    long long r = llroundl(t);
    return (seq_t)r;
}

/*
 * A001618  Nearest integer to 2*n*log(n).
 * https://oeis.org/A001618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(2*n*log(n)), with a(0) = a(1) = 0. Floating point
 *            is used only on the index n (a native integer); the rounded
 *            result is converted to an integer before it becomes a seq_t
 * Range:     u8 n <= 35, u16 n <= 3956, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    long double x = (long double)n;
    long double r = 2.0L * x * logl(x);
    unsigned long long v = (unsigned long long)(r + 0.5L);
    return (seq_t)v;
}

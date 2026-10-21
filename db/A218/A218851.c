/*
 * A218851  a(n) = 2*floor((n + sin(n)/(2*sin(1/2))) * log(n)) + 1.
 * https://oeis.org/A218851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor((n + sin(n)/(2*sin(1/2))) * log(n)) + 1
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* For n < 1 the behaviour is undefined; we simply return 0 */
    if (n == 0) return 0;

    /* Constant sin(1/2) computed once */
    static const double sin_half = sin(0.5);
    double nd = (double)n;

    /* Compute the real-valued expression */
    double val = (nd + sin(nd) / (2.0 * sin_half)) * log(nd);

    /* Floor to nearest integer (little epsilon to guard against rounding) */
    long long fl = (long long)floor(val + 1e-12);

    /* Convert to seq_t and apply final formula */
    seq_t temp = (seq_t)fl;
    return 2 * temp + 1;
}

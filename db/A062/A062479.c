/*
 * A062479  Integer part of log(n^n)^(1 + log(log(1 + n))).
 * https://oeis.org/A062479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (log(n^n))^(1 + log(log(1 + n))) )
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1, the value is defined to be 0 */
#include <math.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    long double nd = (long double) n;
    /* log(n^n) = n * log(n) */
    long double L = nd * logl(nd);
    /* exponent = 1 + log(log(1 + n)) */
    long double exponent = 1.0L + logl(logl(nd + 1.0L));
    /* L^exponent = exp(exponent * log(L)) */
    long double val = expl(exponent * logl(L));
    /* floor with a small epsilon to guard against rounding errors */
    unsigned long long res = (unsigned long long)(val + 1e-12L);
    return (seq_t)res;
}

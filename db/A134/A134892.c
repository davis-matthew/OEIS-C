/*
 * A134892  a(n) = ceiling(n*exp(sin(n))).
 * https://oeis.org/A134892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*exp(sin(n)))
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    double dn = (double)n;
    double v  = dn * exp(sin(dn));
    long long iv = (long long)ceil(v);
    return (seq_t)iv;
}

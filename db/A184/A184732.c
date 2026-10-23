/*
 * A184732  a(n) = floor(n*r+h), where r=(1+sqrt(5))/2, h=-1/4; complement of
 *          A184733.
 * https://oeis.org/A184732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r + h), where r = (1+sqrt(5))/2, h = -1/4
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is defined for n >= 1, but
                               returning 0 for n == 0 avoids UB if called */
    long double val = (long double)n * PHI - 0.25L;
    unsigned long long tmp = (unsigned long long)floorl(val);
    return (seq_t)tmp;
}

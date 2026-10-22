/*
 * A005671  Nearest integer to tan(n)^2.
 * https://oeis.org/A005671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(tan(n)^2). tan(n)^2 is computed in long double
 *            (glibc tanl reduces large arguments exactly) and rounded to the
 *            nearest integer. The result goes through unsigned long long
 *            before it is converted to seq_t
 * Range:     u8 n <= 10, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    long double t = tanl((long double)n);
    long double v = t * t;
    unsigned long long r = (unsigned long long)floorl(v + 0.5L);
    return (seq_t)r;
}

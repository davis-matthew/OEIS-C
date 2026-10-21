/*
 * A050506  Nearest integer to log(Fibonacci(n)).
 * https://oeis.org/A050506
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*log(phi) - log(sqrt(5))
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (phi = (1+sqrt(5))/2)                                          */
#include <math.h>
#include <stdint.h>

static const long double _log_phi    = logl((1.0L + sqrtl(5.0L)) / 2.0L);
static const long double _log_sqrt5 = 0.5L * logl(5.0L);

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 3; behavior below that does not matter */
    if (n == 3)
        return 1;          /* a(3) = 1 */
/* For n >= 4, use the asymptotic formula accurate to less than 0.5         */
    long double val = (long double)n * _log_phi - _log_sqrt5;
    return (seq_t)llroundl(val);
}

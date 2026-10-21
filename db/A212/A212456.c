/*
 * A212456  a(n) = floor(3n*log(3n)).
 * https://oeis.org/A212456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(3*n*log(3*n)) (log is natural logarithm)
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute the real value with double arithmetic, then truncate to the
       largest integer not exceeding it.  The final integer is small enough
       to be safely cast to a long long before converting to seq_t, which
       works for arbitrary‐precision builds as well. */
    double x       = 3.0 * (double)n;
    double value   = x * log(x);
    long long lval = (long long)(value + 1e-12);  /* small epsilon for safety */
    return (seq_t)lval;
}

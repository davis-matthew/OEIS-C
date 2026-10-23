/*
 * A134902  Ceiling(n*exp(-cos(n))).
 * https://oeis.org/A134902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*exp(-cos(n)))
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* Compute the real value using long double for sufficient precision. */
    long double val = (long double)n * expl(-cosl((long double)n));
    /* The -1e-12L offset guards against tiny rounding errors that could
       cause ceil to return one more than the true integer.  The result
       is safely cast to an unsigned long long (which fits for all
       practically relevant n). */
    unsigned long long tmp = (unsigned long long)ceill(val - 1e-12L);
    return (seq_t)tmp;
}

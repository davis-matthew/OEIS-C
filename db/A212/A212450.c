/*
 * A212450  a(n) = ceiling(n + log(n)).
 * https://oeis.org/A212450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n + log(n))
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* ceil_log = ceil( log(n) )   (natural logarithm) */
    idx_t ceil_log = (idx_t)ceill(logl((long double)n));
    return (seq_t)n + (seq_t)ceil_log;
}

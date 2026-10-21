/*
 * A206807  Position of 3^n when {2^j} and {3^k} are jointly ranked;
 *          complement of A206805.
 * https://oeis.org/A206807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*log_2(3))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>           /* for log2l and long double */

seq_t a(idx_t n)
{
    /* Compute floor(n*log_2(3)) using a long double for enough precision. */
    long double val = (long double)n * log2l((long double)3.0);
    idx_t floor_term = (idx_t)val;   /* truncates towards zero, val>0 */

    return (seq_t)n + (seq_t)floor_term;
}

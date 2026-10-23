/*
 * A121238  a(n) = (-1)^(1+n+A088585(n)).
 * https://oeis.org/A121238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(1 + n + A088585(n))
 * Range:     i8 n <= 52, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 1, a(n) equals the ternary paperfolding sequence
   (A034947) which can be computed as: a(n) = 1 if
   (n / (n & -n)) % 4 == 1, otherwise -1.  a(0) = 1. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* lowest set bit of n */
    idx_t lsb = n & (~n + (idx_t)1);   /* equivalent to n & -n */

    /* quotient when dividing n by its lowest set bit */
    idx_t m = n / lsb;

    return (m % 4 == 1) ? (seq_t)1 : (seq_t)(-1);
}

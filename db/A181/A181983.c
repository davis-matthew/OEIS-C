/*
 * A181983  a(n) = (-1)^(n+1) * n.
 * https://oeis.org/A181983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (-1)^(n+1) * n
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    /* If n is even, the sign is negative; if n is odd, it is positive. */
    if ((n & 1) == 0u)
        val = -val;
    return val;
}

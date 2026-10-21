/*
 * A239229  Euler characteristic of n-holed torus: 2 - 2*n.
 * https://oeis.org/A239229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 - 2*n
 * Range:     i8 n <= 52, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the value in a wider integer type to avoid overflow */
    long long val = 2LL - 2LL * (long long)n;
    return (seq_t)val;
}

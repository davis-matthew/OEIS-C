/*
 * A084060  a(n) = 1/2 + (1-6*n)*(-1)^n/2.
 * https://oeis.org/A084060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1/2 + (1-6*n)*(-1)^n/2 = (1 + (1-6*n)*(-1)^n)/2
 * Range:     i8 n <= 42, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    if ((n & 1u) == 0)
    {
        return (seq_t)1 - (seq_t)3 * ns;  /* a(n) = 1 - 3*n  for even n */
    }
    else
    {
        return (seq_t)3 * ns;            /* a(n) = 3*n   for odd n */
    }
}

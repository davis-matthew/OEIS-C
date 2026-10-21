/*
 * A059855  Period of continued fraction for sqrt(n^2+4), n >= 1.
 * https://oeis.org/A059855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 2 if n is even, a(n) = 5 if n is odd and > 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;
    if ((n & (idx_t)1) == (idx_t)0)
        return (seq_t)2;
    return (seq_t)5;
}

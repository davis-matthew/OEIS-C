/*
 * A238377  Row sums of triangle in A204028.
 * https://oeis.org/A238377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*n^2 + 8*n + 5 + 3*(-1)^n)/8
 * Range:     u8 n <= 17, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t parity = (n & 1) ? (seq_t)2 : (seq_t)8;   /* 5+3*(-1)^n = 2 for odd n, 8 for even n */
    return ( (6*tn*tn + 8*tn + parity) / (seq_t)8 );
}

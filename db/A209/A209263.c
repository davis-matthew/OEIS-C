/*
 * A209263  a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5.
 * https://oeis.org/A209263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 61, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;
    seq_t n3 = n2 * s;
    seq_t n4 = n3 * s;
    seq_t n5 = n4 * s;
    seq_t res = 1;
    res += (seq_t)2 * n2;
    res += (seq_t)3 * n3;
    res += (seq_t)4 * n4;
    res += (seq_t)5 * n5;
    return res;
}

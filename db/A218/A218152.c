/*
 * A218152  a(n) = 1 + n + ((n-1)*n^2)/2.
 * https://oeis.org/A218152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2 + 2*n + 2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 - n2 + (seq_t)2 * nn + (seq_t)2;
    return num / (seq_t)2;
}

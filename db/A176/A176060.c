/*
 * A176060  a(n) = n*(n+1)*(3*n^2+5*n+4)/12.
 * https://oeis.org/A176060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2+5*n+4)/12
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 360, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t t1 = N * (N + 1);
    seq_t t2 = (seq_t)3 * N * N + (seq_t)5 * N + (seq_t)4;
    t1 *= t2;
    return t1 / (seq_t)12;
}

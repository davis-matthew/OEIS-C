/*
 * A064304  Eighth diagonal of triangle A064094.
 * https://oeis.org/A064304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6*n + 20*n^2 + 48*n^3 + 90*n^4 + 132*n^5 + 132*n^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 17, u64 n <= 720,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = (seq_t)132;
    res = res * N + (seq_t)132;
    res = res * N + (seq_t)90;
    res = res * N + (seq_t)48;
    res = res * N + (seq_t)20;
    res = res * N + (seq_t)6;
    res = res * N + (seq_t)1;
    return res;
}

/*
 * A106229  Least j > 1 for n > 0 such that j^2 = (n^2 + 1)*(k^2) + (n^2 +
 *          1)*k + 1 where k sequence = A106230.
 * https://oeis.org/A106229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 5, a(2) = 19, for n > 2: a(n) = n^3 - 2*n^2 + n - 1
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)19;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t val = n3 - (seq_t)2 * n2 + nn - (seq_t)1;
    return val;
}

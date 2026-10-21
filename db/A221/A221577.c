/*
 * A221577  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 3*n^6 - 7*n^5 + 29*n^4 - 41*n^3 + 45*n^2 - 33*n +
 *            19 (for n>2) ; a(1) = 16, a(2) = 345
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)16;
    if (n == (idx_t)2) return (seq_t)345;
    seq_t res = (seq_t)1;
    res = res * (seq_t)n + (seq_t)3;
    res = res * (seq_t)n - (seq_t)7;
    res = res * (seq_t)n + (seq_t)29;
    res = res * (seq_t)n - (seq_t)41;
    res = res * (seq_t)n + (seq_t)45;
    res = res * (seq_t)n - (seq_t)33;
    res = res * (seq_t)n + (seq_t)19;
    return res;
}

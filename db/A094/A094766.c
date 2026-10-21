/*
 * A094766  Trajectory of 11 under repeated application of the map n -> n +
 *          2*square excess of n (see A094765).
 * https://oeis.org/A094766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=11, a(1)=15, a(2)=27, a(n)=n^2+5*n+7 for n>2
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)27;
    seq_t m = (seq_t)n;          /* convert index to seq_t */
    return m * m + (seq_t)5 * m + (seq_t)7;
}

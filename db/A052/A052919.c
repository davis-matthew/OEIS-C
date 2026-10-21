/*
 * A052919  a(n) = 1 + 2*3^(n-1) with a(0)=2.
 * https://oeis.org/A052919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*3^(n-1) for n>0, a(0)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    seq_t pow3 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow3 *= (seq_t)3;
    }
    return (seq_t)1 + (seq_t)2 * pow3;
}

/*
 * A184535  a(n) = floor(3/5 * n^2), with a(1)=1.
 * https://oeis.org/A184535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3/5 * n^2), with a(1) = 1
 * Range:     u8 n <= 20, u16 n <= 330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    return (seq_t)(((seq_t)3 * nn * nn) / 5);
}

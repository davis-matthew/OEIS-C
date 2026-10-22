/*
 * A002309  Sum of fourth powers of first n odd numbers.
 * https://oeis.org/A002309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (48*n^5 - 40*n^3 + 7*n)/15
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n5 = n3 * n2;
    
    return (48 * n5 - 40 * n3 + 7 * N) / 15;
}

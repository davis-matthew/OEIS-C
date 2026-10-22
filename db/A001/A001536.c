/*
 * A001536  a(n) = (11*n+1)*(11*n+10).
 * https://oeis.org/A001536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n+1)*(11*n+10)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t a1 = 11 * x + 1;
    seq_t a2 = 11 * x + 10;
    return a1 * a2;
}

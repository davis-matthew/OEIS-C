/*
 * A001512  a(n) = (5*n+1)*(5*n+2)*(5*n+3)*(5*n+4).
 * https://oeis.org/A001512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+1)*(5*n+2)*(5*n+3)*(5*n+4)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t a1 = 5*x + 1;
    seq_t a2 = 5*x + 2;
    seq_t a3 = 5*x + 3;
    seq_t a4 = 5*x + 4;
    return a1 * a2 * a3 * a4;
}

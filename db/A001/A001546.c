/*
 * A001546  a(n) = (8*n+1)*(8*n+3)*(8*n+5)*(8*n+7).
 * https://oeis.org/A001546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+1)*(8*n+3)*(8*n+5)*(8*n+7)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 31, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return (8*x + 1) * (8*x + 3) * (8*x + 5) * (8*x + 7);
}

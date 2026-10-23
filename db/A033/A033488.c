/*
 * A033488  a(n) = n*(n+1)*(n+2)*(n+3)/6.
 * https://oeis.org/A033488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 269, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1) * (nn + 2) * (nn + 3);
    return prod / 6;
}

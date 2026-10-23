/*
 * A016901  a(n) = (5*n + 4)^5.
 * https://oeis.org/A016901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^5
 * Range:     u8 none, u16 n <= 1, u32 n <= 16, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)5 * (seq_t)n + 4;
    seq_t res = base * base * base * base * base;
    return res;
}

/*
 * A017012  a(n) = (7*n + 2)^8.
 * https://oeis.org/A017012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 36, u128 n <= 9361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = 7 * (seq_t)n + 2;
    seq_t r2  = base * base;   // base^2
    seq_t r4  = r2 * r2;       // base^4
    seq_t r8  = r4 * r4;       // base^8
    return r8;
}

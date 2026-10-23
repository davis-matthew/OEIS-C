/*
 * A017141  a(n) = (8*n+6)^5.
 * https://oeis.org/A017141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 9, u64 n <= 890, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)8 * n + 6;
    seq_t r = base;
    r *= base;
    r *= base;
    r *= base;
    r *= base;
    return r;
}

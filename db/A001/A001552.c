/*
 * A001552  a(n) = 1^n + 2^n + ... + 5^n.
 * https://oeis.org/A001552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..5} k^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2 = 1, p3 = 1, p4 = 1, p5 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        p2 *= 2;
        p3 *= 3;
        p4 *= 4;
        p5 *= 5;
    }
    seq_t s = 1;
    s += p2;
    s += p3;
    s += p4;
    s += p5;
    return s;
}

/*
 * A046698  a(0) = 0, a(1) = 1, a(n) = a(a(n-1)) + a(a(n-2)) if n > 1.
 * https://oeis.org/A046698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)) + a(a(n-2)) a(0) = 0, a(1) = 1, a(2) = 1, a(n)
 *            = 2 for n > 2
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    return (seq_t)2;
}

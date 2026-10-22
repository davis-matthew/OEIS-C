/*
 * A002517  Earliest sequence with a(a(n))=3n.
 * https://oeis.org/A002517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(3n) = 3*a(n), a(3n+1) = 3n+2, a(3n+2) = 9n+3
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    unsigned r = n % 3;
    if (r == 1) {
        return (seq_t)n + (seq_t)1;
    } else if (r == 2) {
        return (seq_t)3 * (seq_t)n - (seq_t)3;
    } else {
        return (seq_t)3 * a(n / 3);
    }
}

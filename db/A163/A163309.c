/*
 * A163309  a(n) = 18*a(n-1) - 76*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 76*a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)10;
    if (n == (idx_t)0) return a0;
    if (n == (idx_t)1) return a1;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t next = (seq_t)18 * a1 - (seq_t)76 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}

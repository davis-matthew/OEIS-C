/*
 * A004191  Expansion of 1/(1 - 12*x + x^2).
 * https://oeis.org/A004191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2), a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1;
    seq_t cur = 12;
    if (n == 0) return prev;
    for (idx_t i = 1; i < n; i++) {
        seq_t next = cur * 12 - prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

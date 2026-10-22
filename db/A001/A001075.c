/*
 * A001075  a(0) = 1, a(1) = 2, a(n) = 4*a(n-1) - a(n-2).
 * https://oeis.org/A001075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1;
    seq_t cur = 2;
    idx_t i;
    if (n == 0) return prev;
    for (i = 1; i < n; i++) {
        seq_t next = 4 * cur - prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

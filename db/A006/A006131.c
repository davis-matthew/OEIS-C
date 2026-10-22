/*
 * A006131  a(n) = a(n-1) + 4*a(n-2), a(0) = a(1) = 1.
 * https://oeis.org/A006131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1, cur = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = cur + 4 * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

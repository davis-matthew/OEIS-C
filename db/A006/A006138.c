/*
 * A006138  a(n) = a(n-1) + 3*a(n-2).
 * https://oeis.org/A006138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1;   /* a(0) */
    seq_t cur = 2;    /* a(1) */
    if (n == 0) {
        return prev;
    }
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = cur + 3 * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

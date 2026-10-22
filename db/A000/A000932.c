/*
 * A000932  a(n) = a(n-1) + n*a(n-2); a(0) = a(1) = 1.
 * https://oeis.org/A000932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + n*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1; /* a(i-2) */
    seq_t cur = 1;  /* a(i-1) */
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        seq_t next = cur + (seq_t)i * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

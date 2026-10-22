/*
 * A005592  a(n) = F(2n+1) + F(2n-1) - 1.
 * https://oeis.org/A005592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 1, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1;   /* a(0) */
    seq_t cur = 2;    /* a(1) */
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = 3 * cur - prev + 1;
        prev = cur;
        cur = next;
    }
    return cur;
}

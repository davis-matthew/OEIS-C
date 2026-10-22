/*
 * A007752  Odd bisection of A007750.
 * https://oeis.org/A007752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2) + 8, a(1) = 1, a(2) = 24
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = (seq_t)1;
    seq_t cur = (seq_t)24;
    idx_t i;

    if (n <= 1) return prev;

    for (i = 3; i <= n; i++) {
        seq_t next = cur * 16 - prev + 8;
        prev = cur;
        cur = next;
    }
    return cur;
}

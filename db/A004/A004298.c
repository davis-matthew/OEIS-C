/*
 * A004298  Expansion of (1+2*x+x^2)/(1-66*x+x^2).
 * https://oeis.org/A004298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-1) - a(n-2) for n > 2, a(0) = 1, a(1) = 68, a(2) =
 *            4488
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev, cur, next;
    idx_t i;
    if (n == 0) return (seq_t)1;
    prev = (seq_t)68;
    if (n == 1) return prev;
    cur = prev * 66;          /* a(2) = 4488 */
    for (i = 3; i <= n; i++) {
        next = cur * 66 - prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

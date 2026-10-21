/*
 * A085447  a(n) = 6*a(n-1) + a(n-2), starting with a(0)=2 and a(1)=6.
 * https://oeis.org/A085447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2), with a(0) = 2, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)6;

    seq_t prev = (seq_t)2;   /* a(0) */
    seq_t curr = (seq_t)6;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = curr * (seq_t)6 + prev; /* a(i) = 6*a(i-1) + a(i-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}

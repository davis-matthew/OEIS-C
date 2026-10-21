/*
 * A053573  a(n) = 5*a(n-1) + 14*a(n-2), a(0)=1, a(1)=5.
 * https://oeis.org/A053573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 14*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)5;
    }
    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)5; /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)5 * prev + (seq_t)14 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}

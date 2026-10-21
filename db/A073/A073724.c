/*
 * A073724  a(n) = (4^(n+1) + 6n + 5)/9.
 * https://oeis.org/A073724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3), with a(0)=1, a(1)=3,
 *            a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    seq_t a0 = 1, a1 = 3, a2 = 9;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 6 * a2 - 9 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}

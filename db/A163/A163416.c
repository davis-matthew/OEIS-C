/*
 * A163416  a(n) = 20*a(n-1) - 98*a(n-2) for n>1, a(0)=1, a(1)=14.
 * https://oeis.org/A163416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 98*a(n-2), a(0)=1, a(1)=14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)14;
    const seq_t c20 = (seq_t)20;
    const seq_t c98 = (seq_t)98;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = c20 * a1 - c98 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

/*
 * A135541  a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3), with a(0) = 2, a(1) = 2.
 * https://oeis.org/A135541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3), a(0)=0, a(1)=2, a(2)=7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)7;
    seq_t an = a2;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)2 * a2 - a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}

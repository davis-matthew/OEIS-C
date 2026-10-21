/*
 * A052935  Expansion of (2-2*x-x^3)/((1-2*x)*(1-x^3)).
 * https://oeis.org/A052935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - 1, with a(0)=2, a(1)=2,
 *            a(2)=4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)2;   // a(n-3)
    seq_t a1 = (seq_t)2;   // a(n-2)
    seq_t a2 = (seq_t)4;   // a(n-1)
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 + a1 + (seq_t)2 * a0 - (seq_t)1;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

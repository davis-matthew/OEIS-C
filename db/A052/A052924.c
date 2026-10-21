/*
 * A052924  Expansion of g.f.: (1-x)/(1 - 3*x - x^2).
 * https://oeis.org/A052924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2), with a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)2;          /* a(1) */
    seq_t an = a1;
    for (idx_t i = 2; i <= n; ++i) {
        an = 3 * a1 + a0;        /* a(i) = 3*a(i-1) + a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return an;
}

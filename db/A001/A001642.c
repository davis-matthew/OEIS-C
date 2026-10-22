/*
 * A001642  A Fielder sequence.
 * https://oeis.org/A001642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) + a(n-5), a(1) = 1, a(2) = 3,
 *            a(3) = 4, a(4) = 11, a(5) = 21 (from G.f.
 *            x(1+2x+4x^3+5x^4)/(1-x-x^2-x^4-x^5))
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a1, a2, a3, a4, a5, t;
    idx_t i;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)11;
    if (n == 5) return (seq_t)21;
    /* a1 = a(i-1), a2 = a(i-2), ..., a5 = a(i-5) */
    a1 = 21; a2 = 11; a3 = 4; a4 = 3; a5 = 1;
    for (i = 6; i <= n; i++) {
        t = a1 + a2 + a4 + a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = t;
    }
    return a1;
}

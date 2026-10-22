/*
 * A000352  One half of the number of permutations of [n] such that the
 *          differences have three runs with the same signs.
 * https://oeis.org/A000352
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 17*a(n-3) - 6*a(n-4), a(4) = 5,
 *            a(5) = 29, a(6) = 118, a(7) = 418 (from G.f.
 *            x^4*(5-6*x)/((1-3*x)*(1-2*x)*(1-x)^2), equivalently a(n) =
 *            (3^n-4*2^n-2*n+11)/4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a4 = (seq_t)5, a5 = (seq_t)29, a6 = (seq_t)118, a7 = (seq_t)418;
    idx_t k;
    if (n <= 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;
    if (n == 7) return a7;
    for (k = 8; k <= n; k++) {
        seq_t pos = (seq_t)7 * a7 + (seq_t)17 * a5;
        seq_t neg = (seq_t)17 * a6 + (seq_t)6 * a4;
        seq_t next = pos - neg;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = next;
    }
    return a7;
}

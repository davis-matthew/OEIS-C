/*
 * A052644  Expansion of e.g.f. (1+3x-3x^2)/(1-x)^2.
 * https://oeis.org/A052644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0, otherwise a(n) = (n+4) * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return fact * ((seq_t)n + (seq_t)4);
}

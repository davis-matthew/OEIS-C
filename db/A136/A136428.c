/*
 * A136428  First differences of A064770.
 * https://oeis.org/A136428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*A010052(m mod 10) - 1)*rounded(((1+A010052(m mod
 *            10))*10^k)/3) where n = m*10^k - 1 with m mod 10 > 0
 * Range:     i8 n <= 98, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n + 1;
    seq_t t_copy = t;
    idx_t k = 0;
    while (t_copy % (seq_t)10 == 0) {
        t_copy /= (seq_t)10;
        ++k;
    }
    seq_t d = t_copy % (seq_t)10;
    seq_t u = (d == (seq_t)1 || d == (seq_t)4 || d == (seq_t)9) ? (seq_t)1 : (seq_t)0;
    seq_t pow10k = 1;
    for (idx_t i = 0; i < k; ++i) {
        pow10k *= (seq_t)10;
    }
    seq_t N = ((seq_t)1 + u) * pow10k;
    seq_t mult = (N + (seq_t)1) / (seq_t)3;
    seq_t coeff = ((seq_t)2 * u) - (seq_t)1;
    return coeff * mult;
}

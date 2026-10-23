/*
 * A184362  G.f.: eta(x) + x*eta'(x).
 * https://oeis.org/A184362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; For n>0, a(n) equals (-1)^k * (k*(3k-1)/2 + 1) if n =
 *            k*(3k-1)/2, (-1)^k * (k*(3k+1)/2 + 1) if n = k*(3k+1)/2,
 *            otherwise a(n)=0
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;
        if (g1 > n && g2 > n)
            break;
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
        if (g1 == n) {
            idx_t coeff_idx = k * (3 * k - 1) / 2 + 1;
            seq_t coeff = coeff_idx;
            result += sign * coeff;
        }
        if (g2 == n) {
            idx_t coeff_idx = k * (3 * k + 1) / 2 + 1;
            seq_t coeff = coeff_idx;
            result += sign * coeff;
        }
    }
    return result;
}

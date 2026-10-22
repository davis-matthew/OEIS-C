/*
 * A006272  Denominators of a continued fraction for 1 + sqrt(2).
 * https://oeis.org/A006272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)*(m(n)^2-3)*(m(n)+1), m(n+1) = m(n)^3 - 3*m(n),
 *            a(0) = 12, m(0) = 6 (equivalently a(n) = 2*m(n)*Prod_{k<n}
 *            (m(k)+1))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 12;
    seq_t m = 6;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = m * m - 3;
        v = v * t * (m + 1);
        m = m * t;
    }
    return v;
}

/*
 * A042002  Numerators of continued fraction convergents to sqrt(524).
 * https://oeis.org/A042002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent for sqrt(524)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned period[20] = {
        1, 8, 5, 1, 1, 1, 1, 2, 1, 10,
        1, 2, 1, 1, 1, 1, 5, 8, 1, 44
    };
    seq_t pprev2 = 0;
    seq_t pprev1 = 1;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? 22 : (seq_t)period[(i - 1) % 20];
        seq_t cur = ai * pprev1 + pprev2;
        pprev2 = pprev1;
        pprev1 = cur;
    }
    return pprev1;
}

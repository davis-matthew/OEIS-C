/*
 * A014542  Inverse of 533rd cyclotomic polynomial.
 * https://oeis.org/A014542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{s∈{0,13,41,54}} ε_s * (floor((n−s)/533)+1), with ε =
 *            {1,-1,-1,1} and floor((m)/533)+1 taken as 0 if m<0. This is the
 *            coefficient sequence of 1/Φ_533(x). The sequence is periodic
 *            with period 533
 * Range:     i8 n <= 20, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 533;
    static const idx_t offsets[4] = {0, 13, 41, 54};
    static const int   coeffs[4]  = {1, -1, -1, 1};
    seq_t sum = 0;
    seq_t sn  = (seq_t)n;

    for (int i = 0; i < 4; ++i) {
        seq_t m = sn - (seq_t)offsets[i];
        if (m >= 0) {
            seq_t term = (m / (seq_t)P) + 1;
            sum += (seq_t)coeffs[i] * term;
        }
    }
    return sum;
}

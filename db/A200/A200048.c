/*
 * A200048  Number of real singularities on a family of degree-3n algebraic
 *          surfaces.
 * https://oeis.org/A200048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/12)*(135*n^3-126*n^2 + 45*n-6) if n odd, a(n) =
 *            (1/12)*(135*n^3-117*n^2+54*n-12) if n even
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 390, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t tn  = (seq_t)n;
    seq_t tn2 = tn * tn;
    seq_t tn3 = tn2 * tn;
    seq_t num;
    if (n & 1) {
        num = (seq_t)135 * tn3 - (seq_t)126 * tn2 + (seq_t)45 * tn - (seq_t)6;
    } else {
        num = (seq_t)135 * tn3 - (seq_t)117 * tn2 + (seq_t)54 * tn - (seq_t)12;
    }
    return num / (seq_t)12;
}

/*
 * A128499  Fifth column (m=4) of triangle A128494.
 * https://oeis.org/A128499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) =
 *            ((2*n^4+44*n^3+334*n^2+1012*n+993)*(-1)^((2*n-1+(-1)^n)/4) +
 *            (4*n^3+66*n^2+332*n+495)*(-1)^((6*n-1+(-1)^n)/4) + 48)/1536
 * Range:     i8 none, i16 n <= 6, i32 n <= 237, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t half = N >> 1;            /* floor(n/2) */

    /* Sign for the first polynomial part */
    int p1 = (int)(half & 1);
    int sign1 = p1 ? -1 : 1;

    /* Sign for the second polynomial part */
    int p2;
    if ((N & 1) == 0) {            /* n even */
        p2 = (int)(half & 1);
    } else {                       /* n odd */
        p2 = (int)(((half + 1) & 1));
    }
    int sign2 = p2 ? -1 : 1;

    seq_t n2 = N * N;               /* n^2 */
    seq_t n3 = n2 * N;              /* n^3 */
    seq_t n4 = n3 * N;              /* n^4 */

    seq_t term1 = 2 * n4 + 44 * n3 + 334 * n2 + 1012 * N + 993;
    seq_t term2 = 4 * n3 + 66 * n2 + 332 * N + 495;

    seq_t res = (term1 * sign1 + term2 * sign2 + 48) / 1536;
    return res;
}

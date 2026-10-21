/*
 * A225018  Number of cusps in a class of degree-3n complex algebraic
 *          surfaces.
 * https://oeis.org/A225018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n^3 - 9*n^2 + 4*n - 1)/2 if n is odd; * a(n) =
 *            (12*n^3 - 12*n^2 + 7*n - 2)/2 if n is even. * This implements
 *            OEIS sequence A225018
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* No need to handle n == 0 explicitly; behaviour below offset is irrelevant. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;        /* cast to seq_t to avoid mixed-width ops */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */

    seq_t val;
    if ((n & 1u) == 0) {         /* n even */
        /* val = 12*n^3 - 12*n^2 + 7*n - 2 */
        val = 12 * n3;
        val -= 12 * n2;
        val += 7 * nn;
        val -= 2;
    } else {                     /* n odd */
        /* val = 12*n^3 - 9*n^2 + 4*n - 1 */
        val = 12 * n3;
        val -= 9 * n2;
        val += 4 * nn;
        val -= 1;
    }

    return val / 2;              /* numerator is always even */
}

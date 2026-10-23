/*
 * A182260  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w<x+y.
 * https://oeis.org/A182260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 + (-1)^n - 2*n^2 + 4*n^3)/8 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute n^2 and n^3 in seq_t */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    if ((n & 1) == 0) {               /* n even   */
        /* a = (4*n^3 - 2*n^2)/8 = (2*n^3 - n^2)/4 */
        seq_t t = (seq_t)2 * n3 - n2; /* numerator: 2*n^3 - n^2 */
        return t / (seq_t)4;
    } else {                           /* n odd    */
        /* a = (4*n^3 - 2*n^2 - 2)/8 = (2*n^3 - n^2 - 1)/4 */
        seq_t t = (seq_t)2 * n3 - n2 - (seq_t)1;
        return t / (seq_t)4;
    }
}

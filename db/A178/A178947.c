/*
 * A178947  Expansion of x*(1+2*x+8*x^2+4*x^3+3*x^4) / ( (1+x)^2*(x-1)^4 ).
 * https://oeis.org/A178947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^3 - 3*n^2 + 2*n)/4 for even n, a(n) = (3*n^3 - 3*n^2
 *            + 5*n - 1)/4 for odd n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against zero */
    seq_t tn  = (seq_t)n;           /* convert index to seq_t */
    seq_t tn2 = tn * tn;            /* n^2 */
    seq_t tn3 = tn2 * tn;           /* n^3 */

    if ((n & 1ULL) == 0) {          /* even n */
        return (3 * tn3 - 3 * tn2 + 2 * tn) / 4;
    } else {                        /* odd n */
        return (3 * tn3 - 3 * tn2 + 5 * tn - 1) / 4;
    }
}

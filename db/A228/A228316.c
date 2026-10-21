/*
 * A228316  The hyper-Wiener index of the Cartesian product of the cycles C(n)
 *          and C(n) (a Torus Grid Graph).
 * https://oeis.org/A228316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2-1)*(7*n^2+12*n-3)/96 for n odd; a(n) =
 *            n^4*(7*n^2+12*n+8)/96 for n even
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t res;

    if (n & 1U) {                  /* odd n */
        seq_t n2 = nn * nn;                 /* n^2 */
        seq_t term1 = n2 * (n2 - (seq_t)1); /* n^2*(n^2-1) */
        seq_t poly = (seq_t)7 * n2 + (seq_t)12 * nn - (seq_t)3;
        res = term1 * poly;               /* numerator */
    } else {                       /* even n */
        seq_t n2 = nn * nn;                 /* n^2 */
        seq_t n4 = n2 * n2;                 /* n^4 */
        seq_t poly = (seq_t)7 * n2 + (seq_t)12 * nn + (seq_t)8;
        res = n4 * poly;                   /* numerator */
    }
    /* denominator 96 = 3*32 */
    res = res / (seq_t)96;
    return res;
}

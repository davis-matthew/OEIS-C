/*
 * A186235  Total Wiener index of double-star trees with n nodes.
 * https://oeis.org/A186235
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(28*n^2-129*n+176) + 3*(5*n^2-12*n+8)*(-1)^n - 72)/48
 * Range:     u8 none, u16 n <= 15, u32 n <= 709, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined for n >= 4 with offset 4 (a(4)=10). */
seq_t a(idx_t n)
{
    if (n < 4) return 0; /* undefined for n < 4, harmless */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    /* term1 = n * (28*n^2 - 129*n + 176) */
    seq_t term1 = nn * ( (seq_t)28 * n2 - (seq_t)129 * nn + (seq_t)176 );
    /* term2 = 3 * (5*n^2 - 12*n + 8) */
    seq_t term2 = (seq_t)3 * ( (seq_t)5 * n2 - (seq_t)12 * nn + (seq_t)8 );

    seq_t numer;
    if ((n & 1U) == 0) {               /* even n: (+1)^n =  1 */
        numer = term1 + term2 - (seq_t)72;
    } else {                           /* odd n:  (+1)^n = -1 */
        numer = term1 - term2 - (seq_t)72;
    }
    return numer / (seq_t)48;
}

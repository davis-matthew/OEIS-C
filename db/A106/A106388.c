/*
 * A106388  Numbers k such that 11k = 6j^2 + 6j + 1.
 * https://oeis.org/A106388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (66*n*(n-1) - 21*(2*n-1)*(-1)^n + 23)/4
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote idx_t to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t term1 = 66 * nn * (nn - 1);           /* 66*n*(n-1) */
    seq_t two_n_minus1 = 2 * nn - 1;            /* 2*n-1 */
    seq_t term2 = 21 * two_n_minus1;            /* 21*(2*n-1) */
    seq_t numerator;

    /* (-1)^n is +1 for even n, -1 for odd n */
    if ((n & 1) == 0)   /* n even: subtract term2 */
        numerator = term1 - term2 + 23;
    else                /* n odd: add term2 */
        numerator = term1 + term2 + 23;

    return numerator / 4;
}

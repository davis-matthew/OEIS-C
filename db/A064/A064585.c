/*
 * A064585  a(n) = n^24*(n^2-1)*(n^6+1)*(n^8-1)*(n^12+1).
 * https://oeis.org/A064585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^24*(n^2-1)*(n^6+1)*(n^8-1)*(n^12+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t nn = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = nn * nn;             /* n^2 */
    seq_t n6 = n2 * n2 * n2;        /* n^6 */
    seq_t n8 = n6 * n2;             /* n^8 */
    seq_t n12 = n6 * n6;            /* n^12 */
    seq_t n24 = n12 * n12;          /* n^24 */

    /* Compute the product according to the formula */
    seq_t res = n24;
    res *= (n2 - (seq_t)1);        /* (n^2 - 1) */
    res *= (n6 + (seq_t)1);        /* (n^6 + 1) */
    res *= (n8 - (seq_t)1);        /* (n^8 - 1) */
    res *= (n12 + (seq_t)1);       /* (n^12 + 1) */

    return res;
}

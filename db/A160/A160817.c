/*
 * A160817  Expansion of (1+62*x+564*x^2+1041*x^3+476*x^4+51*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*n^6/20 + 151*n^5/15 + 37*n^4/2 + 19*n^3 + 249*n^2/20 +
 *            74*n/15 + 1 Equivalently a(n) = (183*n^6 + 604*n^5 + 1110*n^4 +
 *            1140*n^3 + 747*n^2 + 296*n + 60) / 60
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1349,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a wider integer type for intermediate calculations,
       this keeps the result exact for any n that fits in seq_t. */
    unsigned __int128 nn = (unsigned __int128)(seq_t)n;

    /* compute powers of n */
    unsigned __int128 n2 = nn * nn;          /* n^2        */
    unsigned __int128 n3 = n2 * nn;          /* n^3        */
    unsigned __int128 n4 = n3 * nn;          /* n^4        */
    unsigned __int128 n5 = n4 * nn;          /* n^5        */
    unsigned __int128 n6 = n5 * nn;          /* n^6        */

    /* numerator of the rational form */
    unsigned __int128 numerator =
          183u * n6
        + 604u * n5
        + 1110u * n4
        + 1140u * n3
        + 747u * n2
        + 296u * nn
        + 60u;

    /* final division by 60 */
    return (seq_t)(numerator / 60u);
}

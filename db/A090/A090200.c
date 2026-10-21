/*
 * A090200  a(n) = N(7,n), where N(7,x) is the 7th Narayana polynomial.
 * https://oeis.org/A090200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + 21*n^5 + 105*n^4 + 175*n^3 + 105*n^2 + 21*n + 1
 *            Implemented via Horner's rule for efficiency
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;       /* cast index to seq_t */
    seq_t res = 1;             /* coefficient of n^6 */

    res = res * nn + (seq_t)21;
    res = res * nn + (seq_t)105;
    res = res * nn + (seq_t)175;
    res = res * nn + (seq_t)105;
    res = res * nn + (seq_t)21;
    res = res * nn + (seq_t)1;

    return res;
}

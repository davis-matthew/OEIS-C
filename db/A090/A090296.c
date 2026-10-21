/*
 * A090296  a(n) = K_4(n) = Sum_{k>=0} A090285(4,k)*2^k*binomial(n,k). a(n) =
 *          2*(n^4+14*n^3+62*n^2+91*n+21)/3.
 * https://oeis.org/A090296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(n^4 + 14*n^3 + 62*n^2 + 91*n + 21)/3
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;           /* n as seq_t   */
    seq_t n2   = nn * nn;            /* n^2          */
    seq_t n3   = n2 * nn;            /* n^3          */
    seq_t n4   = n3 * nn;            /* n^4          */
    seq_t poly = n4 + (seq_t)14 * n3 + (seq_t)62 * n2 + (seq_t)91 * nn + (seq_t)21;
    poly = poly * (seq_t)2;          /* multiply by 2 */
    return poly / (seq_t)3;          /* divide by 3 (exact) */
}

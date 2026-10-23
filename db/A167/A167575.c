/*
 * A167575  The fifth row of the ED3 array A167572.
 * https://oeis.org/A167575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 144*n^4 + 192*n^3 + 1080*n^2 - 48*n + 105
 * Range:     u8 none, u16 n <= 3, u32 n <= 73, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;          /* n */
    seq_t n2 = n1 * n1;           /* n^2 */
    seq_t n3 = n2 * n1;           /* n^3 */
    seq_t n4 = n2 * n2;           /* n^4 */

    seq_t result = (seq_t)144 * n4;   /* 144*n^4 */
    result += (seq_t)192 * n3;       /* +192*n^3 */
    result += (seq_t)1080 * n2;      /* +1080*n^2 */
    result += (seq_t)105;            /* +105 */
    result -= (seq_t)48 * n1;        /* -48*n */
    return result;
}

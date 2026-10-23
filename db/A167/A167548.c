/*
 * A167548  The fifth row of the ED1 array A167546.
 * https://oeis.org/A167548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*n^4 - 6*n^3 + 63*n^2 - 66*n + 24
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* values below the offset are irrelevant */
    }
    seq_t tn  = (seq_t)n;
    seq_t n2  = tn * tn;          /* n^2 */
    seq_t n3  = n2 * tn;          /* n^3 */
    seq_t n4  = n3 * tn;          /* n^4 */
    seq_t result = 9*n4 - 6*n3 + 63*n2 - 66*tn + 24;
    return result;
}

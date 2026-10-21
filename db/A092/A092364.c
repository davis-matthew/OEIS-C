/*
 * A092364  a(n) = n^2*binomial(n,2).
 * https://oeis.org/A092364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * binomial(n,2) = n^3 * (n-1) / 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;          /* n^2 */
    res = res * nn;               /* n^3 */
    res = res * (nn - (seq_t)1);  /* n^3 * (n-1) */
    res = res / (seq_t)2;         /* divide by 2 */
    return res;
}

/*
 * A055821  a(n) = T(n,n-4), array T as in A055818.
 * https://oeis.org/A055821
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (72 - 54*n - 25*n^2 + 6*n^3 + n^4)/24 for n > 4, a(4) = 1
 *            (A055821)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 564, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 4)
        return 0;

    if (n == 4)
        return 1;          /* a(4) = 1 */

    /* compute (n^4 + 6*n^3 + 72) - (25*n^2 + 54*n) and divide by 24 */
    seq_t nt = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = nt * nt;           /* n^2 */
    seq_t n3 = n2 * nt;           /* n^3 */
    seq_t n4 = n3 * nt;           /* n^4 */

    seq_t lhs = n4 + (seq_t)6 * n3 + (seq_t)72;
    seq_t rhs = (seq_t)25 * n2 + (seq_t)54 * nt;

    seq_t num = lhs - rhs;        /* numerator: always divisible by 24 */
    return num / (seq_t)24;
}

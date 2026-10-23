/*
 * A037252  a(n) = n^12*(n^6 + 1)*(n^4 - 1)*(n^3 + 1)*(n-1).
 * https://oeis.org/A037252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12*(n^6 + 1)*(n^4 - 1)*(n^3 + 1)*(n-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n;

    /* powers of n */
    seq_t n2  = nn * nn;       /* n^2   */
    seq_t n3  = n2 * nn;       /* n^3   */
    seq_t n4  = n2 * n2;       /* n^4   */
    seq_t n6  = n4 * n2;       /* n^6   */
    seq_t n12 = n6 * n6;       /* n^12  */

    seq_t res = n12;
    res *= (n6 + 1);
    res *= (n4 - 1);
    res *= (n3 + 1);
    res *= (nn - 1);           /* n-1   */

    return res;
}

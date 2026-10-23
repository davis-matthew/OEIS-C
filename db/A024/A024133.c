/*
 * A024133  a(n) = 11^n - n^6.
 * https://oeis.org/A024133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - n^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 11^n */
    seq_t pow11 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow11 *= (seq_t)11;

    /* compute n^6 */
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;          /* n^2 */
    seq_t n3 = n2 * tn;          /* n^3 */
    seq_t n4 = n3 * tn;          /* n^4 */
    seq_t n5 = n4 * tn;          /* n^5 */
    seq_t n6 = n5 * tn;          /* n^6 */

    /* final result */
    return pow11 - n6;
}

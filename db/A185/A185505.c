/*
 * A185505  a(n) = (7*n^4 + 5*n^2)/12.
 * https://oeis.org/A185505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^4 + 5*n^2)/12
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 178, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;        /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t num = 7 * n4 + 5 * n2; /* numerator 7*n^4 + 5*n^2 */
    return num / 12;               /* exact division, gives a(n) */
}

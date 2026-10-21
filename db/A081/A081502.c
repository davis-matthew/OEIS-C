/*
 * A081502  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 3x+y.
 * https://oeis.org/A081502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 7*floor(n/10)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) = 3x + y where n = 10x + y, 0 <= y <= 9 */
    /* Equivalent: a(n) = n - 7 * (n / 10)   */
    seq_t sn = (seq_t)n;           /* cast index to sequence type */
    seq_t div10 = sn / 10;         /* integer division truncates */
    return sn - 7 * div10;
}

/*
 * A001014  Sixth powers: a(n) = n^6.
 * https://oeis.org/A001014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;      /* n^2 */
    seq_t x4 = x2 * x2;    /* n^4 */
    return x4 * x2;        /* n^6 */
}

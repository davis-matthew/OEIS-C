/*
 * A135497  a(n) = n^5 - n^2.
 * https://oeis.org/A135497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - n^2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n² */
    seq_t n3 = n2 * nn;        /* n³ */
    seq_t n5 = n3 * n2;        /* n⁵ = n³ · n² */
    return n5 - n2;            /* a(n) = n⁵ – n² */
}

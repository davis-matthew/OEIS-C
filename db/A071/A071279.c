/*
 * A071279  Kissing number of regular n-gon.
 * https://oeis.org/A071279
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 12, a(4) = 8, a(n) = 6 for n ≥ 5
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)8;
    if (n >= 5) return (seq_t)6;
    /* For indices below the offset, the value is undefined; return 0. */
    return (seq_t)0;
}

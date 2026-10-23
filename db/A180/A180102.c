/*
 * A180102  a(1)=1; a(n)=percentage of sum of previous terms represented by n.
 * https://oeis.org/A180102
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(200/n) (with a(1)=1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* not used, but safe */
    if (n == 1) return 1;             /* special case */
    return (seq_t)200 / (seq_t)n;     /* integer division gives floor */
}

/*
 * A080341  Sum of the first n terms that are congruent to 1, 4 or 5 mod 6
 *          (A047259).
 * https://oeis.org/A080341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + (n+1)/3 (integer division)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;      /* convert index to sequence type */
    seq_t res  = nn * nn;       /* n^2 */
    res += (nn + 1) / 3;        /* add integer division of (n+1)/3 */
    return res;
}

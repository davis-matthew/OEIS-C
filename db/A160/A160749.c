/*
 * A160749  a(n) = (11*n^2 + 19*n + 10)/2.
 * https://oeis.org/A160749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^2 + 19*n + 10)/2
 * Range:     u8 n <= 3, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;            /* n^2 */
    val *= 11;                      /* 11*n^2 */
    val += nn * 19;                 /* +19*n */
    val += 10;                      /* +10 */
    val /= 2;                       /* divide by 2 */
    return val;
}

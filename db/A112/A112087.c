/*
 * A112087  a(n) = 4*(n^2 - n + 1).
 * https://oeis.org/A112087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n^2 - n + 1)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;   /* n^2 */
    val -= nn;             /* n^2 - n */
    val += 1;              /* n^2 - n + 1 */
    val *= 4;              /* 4*(n^2 - n + 1) */
    return val;
}

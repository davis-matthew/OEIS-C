/*
 * A010802  14th powers: a(n) = n^14.
 * https://oeis.org/A010802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^14
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;          /* n converted to seq_t */
    seq_t a2   = base * base;       /* n^2 */
    seq_t a4   = a2 * a2;           /* n^4 */
    seq_t a8   = a4 * a4;           /* n^8 */
    return a8 * a4 * a2;            /* n^8 * n^4 * n^2 = n^14 */
}

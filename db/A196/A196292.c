/*
 * A196292  a(n) = n^10 + n.
 * https://oeis.org/A196292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 + n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns  = (seq_t)n;     /* n as seq_t */
    seq_t n2  = ns * ns;      /* n^2 */
    seq_t n4  = n2 * n2;      /* n^4 */
    seq_t n8  = n4 * n4;      /* n^8 */
    seq_t pow10 = n8 * n2;    /* n^10 */
    return pow10 + ns;        /* n^10 + n */
}

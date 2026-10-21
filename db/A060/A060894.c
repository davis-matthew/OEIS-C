/*
 * A060894  a(n) = n^8 + n^7 - n^5 - n^4 - n^3 + n + 1.
 * https://oeis.org/A060894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + n^7 - n^5 - n^4 - n^3 + n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t n2  = x * x;           /* n^2 */
    seq_t n3  = n2 * x;          /* n^3 */
    seq_t n4  = n2 * n2;         /* n^4 */
    seq_t n5  = n4 * x;          /* n^5 */
    seq_t n7  = n3 * n4;         /* n^7 */
    seq_t n8  = n4 * n4;         /* n^8 */

    /* compute (n^8 + n^7 + n + 1) - (n^5 + n^4 + n^3) */
    return (n8 + n7 + x + (seq_t)1) - (n5 + n4 + n3);
}

/*
 * A190780  a(n) = 2*(n^8 + 224*n^4 + 256)^2.
 * https://oeis.org/A190780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^8 + 224*n^4 + 256)^2
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 15, u128 n <= 245,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;            /* n */
    seq_t n2  = nn * nn;             /* n^2 */
    seq_t n4  = n2 * n2;             /* n^4 */
    seq_t n8  = n4 * n4;             /* n^8 */
    seq_t val = n8 + (seq_t)224 * n4 + (seq_t)256; /* n^8 + 224*n^4 + 256 */
    return (seq_t)2 * val * val;     /* 2 * (val)^2 */
}

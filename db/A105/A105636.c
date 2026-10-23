/*
 * A105636  Transform of n^3 by the Riordan array (1/(1-x^2), x).
 * https://oeis.org/A105636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 + 8*n^3 + 8*n^2 - 1 + (-1)^n)/16
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n3  = n2 * nn;           /* n^3 */
    seq_t n4  = n2 * n2;           /* n^4 */
    seq_t base = 2 * n4 + 8 * n3 + 8 * n2; /* 2n^4 + 8n^3 + 8n^2 */
    if (n & 1)                     /* n odd: subtract 2 */
        base -= 2;
    return base / 16;              /* exact division by 16 */
}

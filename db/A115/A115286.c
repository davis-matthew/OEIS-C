/*
 * A115286  a(n) = (1/6)*(n^6+3*n^4+12*n^3+8*n^2).
 * https://oeis.org/A115286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 3*n^4 + 12*n^3 + 8*n^2) / 6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* n as seq_t */
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n2 * n2;       /* n^4 */
    seq_t n6 = n3 * n3;       /* n^6 */

    seq_t num = n6 + 3 * n4;  /* n^6 + 3*n^4 */
    num += 12 * n3;           /* +12*n^3 */
    num += 8 * n2;            /* +8*n^2 */

    return num / 6;           /* divide by 6 */
}

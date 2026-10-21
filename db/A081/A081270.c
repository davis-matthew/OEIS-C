/*
 * A081270  Diagonal of triangular spiral in A051682.
 * https://oeis.org/A081270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n^2 + 17*n + 6)/2
 * Range:     u8 n <= 4, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn;           /* n^2 */
    num *= (seq_t)9;               /* 9*n^2 */
    num += nn * ((seq_t)17);       /* +17*n */
    num += (seq_t)6;               /* +6 */
    return num / (seq_t)2;         /* divide by 2 */
}

/*
 * A213837  Principal diagonal of the convolution array A213836.
 * https://oeis.org/A213837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5 - 15*n + 16*n^2)/6
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 699, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return 0;               /* offset starts at 1 */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;                 /* n^2 */
    seq_t num = nn * (((seq_t)5) + ((seq_t)16) * n2 - ((seq_t)15) * nn);
    return num / ((seq_t)6);
}

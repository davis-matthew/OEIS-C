/*
 * A137741  Number of different strings of length n+4 obtained from "123...n"
 *          by iteratively duplicating any substring.
 * https://oeis.org/A137741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/24*(n+3)*(n^3+15*n^2+50*n-96) for n>2; a(1)=1; a(2)=16
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;

    seq_t nn   = (seq_t)n;
    seq_t term1 = nn + (seq_t)3;

    /* Compute n^3 + 15*n^2 + 50*n - 96 */
    seq_t n2   = nn * nn;                    /* n^2   */
    seq_t n3   = n2 * nn;                    /* n^3   */
    seq_t term2 = n3;
    term2 += ((seq_t)15) * n2;              /* +15*n^2 */
    term2 += ((seq_t)50) * nn;              /* +50*n   */
    term2 -= ((seq_t)96);                   /* -96     */

    seq_t prod = term1 * term2;
    return prod / (seq_t)24;
}

/*
 * A058919  a(n) = n^4/2 - n^3 + 3*n^2/2 - n + 1 = (n^2 + 1)*(n^2 - 2*n +
 *          2)/2.
 * https://oeis.org/A058919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1) * (n^2 - 2*n + 2) / 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 258, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* intermediate squares and terms */
    seq_t n2      = nn * nn;           /* n^2 */
    seq_t term1   = n2 + (seq_t)1;     /* n^2 + 1 */
    seq_t term2   = n2 - ((seq_t)2 * nn) + (seq_t)2; /* n^2 - 2*n + 2 */

    /* product and final division by 2 */
    seq_t prod    = term1 * term2;     /* (n^2 + 1)*(n^2 - 2*n + 2) */
    return prod / (seq_t)2;
}

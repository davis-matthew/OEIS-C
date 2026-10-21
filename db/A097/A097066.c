/*
 * A097066  Expansion of (1-2*x+2*x^2)/((1+x)*(1-x)^3).
 * https://oeis.org/A097066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? n*n/4 + 1 : (n*n - 1)/4
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the explicit formula derived from A097066. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to sequence type */
    seq_t sq = nn * nn;           /* n^2 */
    if (n & 1)                    /* odd n: a(n) = (n^2 - 1)/4 */
        return (sq - 1) / 4;
    else                          /* even n: a(n) = n^2/4 + 1 */
        return sq / 4 + 1;
}

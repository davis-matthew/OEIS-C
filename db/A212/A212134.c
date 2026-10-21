/*
 * A212134  Number of (w,x,y,z) with all terms in {1,...,n} and median<=mean.
 * https://oeis.org/A212134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 + 2*n^2 - 3*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 275, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t n3 = n2 * ns;           /* n^3 */
    seq_t n4 = n3 * ns;           /* n^4 */
    /* Compute (n4 + 2*n3 + 2*ns - 3*n2) / 2 using only non‑negative additions */
    seq_t term = (n4 + 2 * n3 + 2 * ns - 3 * n2) / (seq_t)2;
    return term;
}

/*
 * A154615  a(n) = A022998(n)^2.
 * https://oeis.org/A154615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ((5 + 3*(-1)^n)/2) * n^2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  = 4*n^2 if n is even, n^2 if n is odd. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;                /* n^2 */
    if ((n & 1) == 0)                   /* n even  */
        res *= (seq_t)4;                /* 4*n^2  */
    return res;                         /* odd => n^2 */
}

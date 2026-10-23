/*
 * A100165  Structured rhombic triacontahedral numbers (vertex structure 7).
 * https://oeis.org/A100165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n-2)^2
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)3 * nn - (seq_t)2;   /* compute (3*n-2) */
    return nn * t * t;                     /* n * (3*n-2)^2 */
}

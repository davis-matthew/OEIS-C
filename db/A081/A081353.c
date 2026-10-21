/*
 * A081353  Diagonal of square maze arrangement of natural numbers A081349.
 * https://oeis.org/A081353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 3n + 3 if n is even; a(n) = n^2 + 3n + 1 if n is
 *            odd
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                 /* convert index to seq_t */
    seq_t prod = nn * (nn + 3);            /* n*(n+3) = n^2 + 3n   */
    if ((n & 1) == 0)                     /* even n */
        return prod + 3;
    else                                   /* odd n  */
        return prod + 1;
}

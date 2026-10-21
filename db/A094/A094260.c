/*
 * A094260  Sum of next n numbers/n if n divides the sum else n times the sum
 *          of next n numbers.
 * https://oeis.org/A094260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2+1)/2, n even (n^2+1)/2, n odd
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                     /* even n */
        seq_t n2 = (seq_t)n * (seq_t)n;     /* n^2  */
        return n2 * (n2 + 1) / 2;           /* n^2*(n^2+1)/2 */
    } else {                                 /* odd n */
        seq_t n2 = (seq_t)n * (seq_t)n;     /* n^2  */
        return (n2 + 1) / 2;                /* (n^2+1)/2     */
    }
}

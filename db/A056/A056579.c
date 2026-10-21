/*
 * A056579  a(n) = 1+2n+3n^2+4n^3+5n^4.
 * https://oeis.org/A056579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2n + 3n^2 + 4n^3 + 5n^4
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)5;          /* start with the leading coefficient 5   */
    result = result * nn + (seq_t)4;  /* 5*n + 4                               */
    result = result * nn + (seq_t)3;  /* (5*n + 4)*n + 3 = 5n^2 + 4n + 3        */
    result = result * nn + (seq_t)2;  /* add the 2n term                        */
    result = result * nn + (seq_t)1;  /* finally add the constant 1             */
    return result;
}

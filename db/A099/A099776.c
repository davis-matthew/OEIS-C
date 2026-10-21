/*
 * A099776  Length of the hypotenuse of an integer right triangle with the
 *          hypotenuse being one more than the longer side. The shorter sides
 *          are just consecutive odd numbers 3, 5, 7, ...
 * https://oeis.org/A099776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 1 + 2*n + 2*n^2
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Offset is 1; n=0 is undefined */
    seq_t nn = (seq_t)n;
    return (seq_t)2 * nn * nn + (seq_t)2 * nn + (seq_t)1;
}

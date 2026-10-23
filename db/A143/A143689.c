/*
 * A143689  a(n) = (3*n^2 - n + 2)/2.
 * https://oeis.org/A143689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 - n + 2)/2
 * Range:     u8 n <= 9, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = (seq_t)3 * nn - (seq_t)1;
    seq_t prod = nn * part;          /* n * (3*n - 1)  */
    seq_t val  = prod / (seq_t)2 + (seq_t)1; /* (n*(3*n-1))/2 + 1 */
    return val;
}

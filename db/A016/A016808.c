/*
 * A016808  a(n) = (4n)^8.
 * https://oeis.org/A016808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)4 * (seq_t)n;   /* x = 4n */
    seq_t x2 = x * x;                 /* x^2 = (4n)^2 */
    seq_t x4 = x2 * x2;               /* x^4 = (4n)^4 */
    seq_t x8 = x4 * x4;               /* x^8 = (4n)^8 */
    return x8;
}

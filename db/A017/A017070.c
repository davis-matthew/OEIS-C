/*
 * A017070  a(n) = (8*n)^6.
 * https://oeis.org/A017070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 203, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t first to avoid premature overflow of smaller types */
    seq_t x = (seq_t)n * (seq_t)8;
    seq_t x2 = x * x;          /* (8*n)^2 */
    seq_t x4 = x2 * x2;        /* (8*n)^4 */
    seq_t res = x4 * x2;       /* (8*n)^6 = (8*n)^4 * (8*n)^2 */
    return res;
}

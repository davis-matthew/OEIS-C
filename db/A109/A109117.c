/*
 * A109117  a(n) = (n+1)^3*(2n+1)(5n+1).
 * https://oeis.org/A109117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^3*(2n+1)(5n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t NN = (seq_t)n;
    seq_t t1 = NN + 1;
    seq_t t2 = t1 * t1 * t1;   /* (n+1)^3 */
    seq_t t3 = 2 * NN + 1;      /* 2n+1 */
    seq_t t4 = 5 * NN + 1;      /* 5n+1 */
    return t2 * t3 * t4;
}

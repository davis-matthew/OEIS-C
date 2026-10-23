/*
 * A139578  a(n) = n*(2*n + 13).
 * https://oeis.org/A139578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n + 13)
 * Range:     u8 n <= 8, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t  = (seq_t)2 * ns + (seq_t)13;
    return ns * t;
}

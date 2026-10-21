/*
 * A086653  a(n) = 2^n + 3*n.
 * https://oeis.org/A086653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = (seq_t)1;
    /* shift left by n bits to compute 2^n */
    pow2 <<= n;
    /* compute 2^n + 3*n */
    return pow2 + (seq_t)3 * n;
}

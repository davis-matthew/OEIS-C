/*
 * A168381  a(n) = 4*n + 2*(-1)^n.
 * https://oeis.org/A168381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*n + 2*(-1)^n
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) using seq_t division by 2 */
    seq_t res = (seq_t)2 + (seq_t)8 * ((seq_t)n / 2);
    return res;
}

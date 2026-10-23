/*
 * A100281  a(n) = A099896(A099896(n)).
 * https://oeis.org/A100281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n XOR ⌊n/4⌋ XOR ⌊n/16⌋
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t q4 = x >> 2;   /* floor(n/4) */
    seq_t q16 = x >> 4;  /* floor(n/16) */
    return x ^ q4 ^ q16;
}

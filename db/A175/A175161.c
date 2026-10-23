/*
 * A175161  a(n) = 8*(2^n + 1).
 * https://oeis.org/A175161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(2^n + 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t t = (seq_t)1;
    t <<= n;               /* t = 2^n  */

    /* a(n) = 8 * (t + 1) */
    return (t + (seq_t)1) * (seq_t)8;
}

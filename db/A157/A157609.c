/*
 * A157609  a(n) = 2662*n - 22.
 * https://oeis.org/A157609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2662*n - 22
 * Range:     u8 none, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to force arithmetic in that type */
    seq_t ns = (seq_t)n;
    return 2662 * ns - 22;
}

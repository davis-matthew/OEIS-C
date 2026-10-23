/*
 * A195320  7 times hexagonal numbers: a(n) = 7*n*(2*n-1).
 * https://oeis.org/A195320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*(2*n-1)
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    return 7 * nn * (2 * nn - 1);
}

/*
 * A153780  10 times pentagonal numbers: a(n) = 5*n*(3*n-1).
 * https://oeis.org/A153780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(3*n-1)
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    return 5 * s * (3 * s - 1);
}

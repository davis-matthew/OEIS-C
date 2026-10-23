/*
 * A121273  Number of different n-dimensional convex regular polytopes that
 *          can tile n-dimensional space.
 * https://oeis.org/A121273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n = 2 or n = 4, otherwise 1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2 || n == 4)
        return (seq_t)3;
    return (seq_t)1;
}

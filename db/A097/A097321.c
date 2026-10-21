/*
 * A097321  a(n) = (3*n-1) * 3*n * (3*n+1).
 * https://oeis.org/A097321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1) * 3*n * (3*n+1)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once and compute m = 3*n safely within seq_t */
    seq_t m = (seq_t)n * 3;
    /* Compute the product of the three consecutive integers */
    return (m - 1) * m * (m + 1);
}

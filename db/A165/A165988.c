/*
 * A165988  First trisection of A022998.
 * https://oeis.org/A165988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3 + (-1)^n)/2 Equivalent: a(n) = 6*n if n is even,
 *            3*n if n is odd
 * Range:     u8 n <= 43, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* If n is even -> 6*n, otherwise -> 3*n */
    return (n & 1ULL) ? N * 3 : N * 6;
}

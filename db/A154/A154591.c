/*
 * A154591  a(n) = 2*n^2 + 18*n + 7.
 * https://oeis.org/A154591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 18*n + 7
 * Range:     u8 n <= 7, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t once to avoid repeated conversions */
    seq_t t = (seq_t)n;
    /* compute 2*n^2 + 18*n + 7 using only integer arithmetic */
    return t * (2u * t + 18u) + 7u;
}

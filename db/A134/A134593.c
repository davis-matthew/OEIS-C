/*
 * A134593  a(n) = 5*n^2 + 10*n + 1. Coefficients of the rational part of (1 +
 *          sqrt(n))^5.
 * https://oeis.org/A134593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 10*n + 1
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return 5*x*x + 10*x + 1;
}

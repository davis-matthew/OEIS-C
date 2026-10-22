/*
 * A001068  a(n) = floor(5*n/4), numbers that are congruent to {0, 1, 2, 3}
 *          mod 5.
 * https://oeis.org/A001068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/4)
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 5 / 4;
}

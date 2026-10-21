/*
 * A062026  a(n) = n*(n+1)*(n^2 - 3*n + 6)/4.
 * https://oeis.org/A062026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 - 3*n + 6)/4
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;
    /* (n*(n+1))/2 is always integer */
    seq_t part1 = n1 * (n1 + 1) / 2;
    /* (n^2 - 3*n + 6) is always even, so divide by 2 safely */
    seq_t temp = n1 * n1 - 3 * n1 + 6;
    seq_t part2 = temp / 2;
    return part1 * part2;
}

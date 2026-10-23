/*
 * A047220  Numbers that are congruent to {0, 1, 3} mod 5.
 * https://oeis.org/A047220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*(n-1)/3)
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n should be >= 1; behaviour for n == 0 is undefined */
    return 5 * ((seq_t)(n - 1)) / 3;
}

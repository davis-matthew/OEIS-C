/*
 * A082369  Numbers congruent to 13 mod 30.
 * https://oeis.org/A082369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*(n-1) + 13
 * Range:     u8 n <= 9, u16 n <= 2185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)(n - 1)) * 30 + 13;
}

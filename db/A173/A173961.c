/*
 * A173961  Averages of two consecutive even cubes: (n^3 + (n+2)^3)/2.
 * https://oeis.org/A173961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 - 12*n^2 + 12*n - 4
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 813, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    return (sn * sn * sn * 8) - (sn * sn * 12) + (sn * 12) - 4;
}

/*
 * A127989  a(n) = 2*n^3 - 2*n + 9.
 * https://oeis.org/A127989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 2*n + 9
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;
    return 2 * x * x * x - 2 * x + 9;
}

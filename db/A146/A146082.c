/*
 * A146082  a(n) = A146081(n) mod 9.
 * https://oeis.org/A146082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (74*n + 110) mod 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 9 to keep values small */
    idx_t m = n % 9;
    /* a(n) = (2*(n + 1)) mod 9 */
    seq_t result = ((seq_t)(m + 1) * 2) % 9;
    return result;
}

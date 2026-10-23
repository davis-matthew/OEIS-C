/*
 * A137934  Period 6: 2,2,2,2,2,0.
 * https://oeis.org/A137934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(gcd(n mod 2, n mod 3), 2)
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((n % 6) == 0) ? (seq_t)0 : (seq_t)2;
}

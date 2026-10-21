/*
 * A064806  a(n) = n + digital root of n.
 * https://oeis.org/A064806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + digital_root(n) , digital_root(n) = 1 + (n-1) mod 9
 *            for n>0
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; undefined below */
    seq_t x = (seq_t)n;
    seq_t dr = x % 9;
    if (dr == 0) dr = 9;            /* digital root of multiples of 9 is 9 */
    return x + dr;
}

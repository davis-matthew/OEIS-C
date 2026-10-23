/*
 * A109048  a(n) = lcm(n,7).
 * https://oeis.org/A109048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,7) = n*7/gcd(n,7)
 * Range:     u8 n <= 36, u16 n <= 9362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n % 7 == 0)
        return (seq_t)n;
    return ((seq_t)n) * (seq_t)7;
}

/*
 * A130724  a(n) = lcm(n,3) / gcd(n,3).
 * https://oeis.org/A130724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n mod 3 == 0) ? n/3 : 3*n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0)
        return (seq_t)(n / 3);
    else
        return (seq_t)n * 3;
}

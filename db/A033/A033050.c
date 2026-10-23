/*
 * A033050  Numbers whose set of base 14 digits is {0,1}.
 * https://oeis.org/A033050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i>=0} bit_i(n) * 14^i, where bit_i(n) is the i‑th
 *            bit of n * (binary representation). This follows the recurrence:
 *            * a(2n) = 14 * a(n) * a(2n+1) = a(2n) + 1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    while (n != 0)
    {
        if (n & 1)
            result += power;
        power *= (seq_t)14;
        n >>= 1;
    }
    return result;
}

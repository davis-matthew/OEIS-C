/*
 * A021041  Decimal expansion of 1/37.
 * https://oeis.org/A021041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth digit of 1/37 decimal expansion (period 3: 0,2,7)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The decimal expansion of 1/37 is 0.027027027..., repeating the pattern 0,2,7. */
    static const uint8_t pattern[3] = {0, 2, 7};
    idx_t mod = n % 3;          /* n can be very large, but modulo 3 is safe */
    return (seq_t)pattern[mod];
}

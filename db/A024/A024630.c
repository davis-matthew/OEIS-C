/*
 * A024630  n written in fractional base 4/2.
 * https://oeis.org/A024630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2 * A007088(floor(n/2)) + (n mod 2) where A007088(k) is
 *            the decimal number whose digits are the binary representation of
 *            k (e.g., k = 13 -> A007088 = 1101)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Convert k to the decimal number formed from its binary digits. */
static seq_t encode_binary_as_decimal(idx_t k)
{
    if (k == 0)
        return 0;
    seq_t res = 0;
    int msb = 0;
    idx_t t = k;
    while (t >>= 1)
        ++msb;                /* find position of most significant bit */

    for (int i = msb; i >= 0; --i)
    {
        res = res * 10 + ((k >> i) & 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t b = encode_binary_as_decimal(n >> 1); /* floor(n/2) */
    return b * 2 + (n & 1);
}

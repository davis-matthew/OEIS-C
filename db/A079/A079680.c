/*
 * A079680  Number of 1's in n!.
 * https://oeis.org/A079680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the number of decimal digit '1' in n!
 * Range:     u8 n <= 970, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 or 1: 0! = 1, 1! = 1 → one digit '1' */
    if (n <= 1) return (seq_t)1;

    /* 10000! has about 36000 decimal digits. 40000 gives safe margin. */
    static uint8_t digits[40000];
    size_t len = 1;            /* current number of digits, least significant first */
    digits[0] = 1;             /* start with factorial = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint32_t prod = (uint32_t)digits[j] * (uint32_t)i + carry;
            digits[j] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry)
        {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    seq_t ones = 0;
    for (size_t j = 0; j < len; ++j)
        if (digits[j] == 1)
            ++ones;

    return ones;
}

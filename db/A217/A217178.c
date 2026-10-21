/*
 * A217178  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains three consecutive identical
 *          digits.
 * https://oeis.org/A217178
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digits in the smallest power of n that contains
 *            three consecutive identical decimal digits. The function
 *            searches powers of n until such a pattern is found
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Indices below the offset are irrelevant */
    if (n < 2) return 0;

    /* Dynamic array of decimal digits (little‑endian) */
    size_t cap = 1024;                /* initial capacity */
    uint8_t *dec = (uint8_t *)malloc(cap);
    if (!dec) return 0;               /* allocation failed */

    dec[0] = 1;                       /* power = n^0 = 1 initially */
    size_t len = 1;                   /* current number of digits */

    for (;;)
    {
        /* Multiply the current number by n (decimal big‑int) */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint64_t tmp = (uint64_t)dec[i] * n + carry;
            dec[i]     = (uint8_t)(tmp % 10);
            carry      = tmp / 10;
        }
        while (carry)
        {
            if (len == cap)              /* grow the array if necessary */
            {
                cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(dec, cap);
                if (!tmp)
                {
                    free(dec);
                    return 0;           /* allocation failed */
                }
                dec = tmp;
            }
            dec[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Scan for any run of three equal digits */
        for (size_t i = 0; i + 2 < len; ++i)
        {
            if (dec[i] == dec[i + 1] && dec[i] == dec[i + 2])
            {
                free(dec);
                return (seq_t)len;       /* number of decimal digits */
            }
        }
    }
}

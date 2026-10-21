/*
 * A091946  a(n) = floor(11^n/10^n).
 * https://oeis.org/A091946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11^n / 10^n)
 * Range:     u8 n <= 58, u16 n <= 116, u32 n <= 232, u64 n <= 465,
 *            u128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Computes a(n) = floor(1.1^n) = floor(11^n / 10^n)   */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    /* Estimate the number of decimal digits of 11^n to allocate
       enough space for the intermediate representation. */
    double est = n * log10(11.0) + 1.5;   /* a little safety margin */
    size_t cap = (size_t)est + 5;
    uint8_t *dig = (uint8_t *)malloc(cap);
    if (!dig)
        return 0;                     /* out of memory – unlikely in tests */

    size_t len = 1;
    dig[0] = 1;                         /* start with 11^0 = 1 */

    /* Multiply by 11 exactly n times, keeping the number in
       decimal digits (least significant digit first). */
    for (idx_t step = 0; step < n; ++step)
    {
        uint32_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint32_t tmp = (uint32_t)dig[i] * 11u + carry;
            dig[i]      = (uint8_t)(tmp % 10u);
            carry       = tmp / 10u;
        }
        while (carry)
        {
            if (len == cap)           /* grow buffer if needed */
            {
                cap <<= 1;
                uint8_t *tmp_ptr = (uint8_t *)realloc(dig, cap);
                if (!tmp_ptr)
                {
                    free(dig);
                    return 0;
                }
                dig = tmp_ptr;
            }
            dig[len++] = (uint8_t)(carry % 10u);
            carry     /= 10u;
        }
    }

    /* Now dropping the last n digits yields floor(11^n / 10^n). */
    seq_t result = 0;
    size_t idx = len;
    while (idx > n)
    {
        result = result * 10 + dig[--idx];
    }

    free(dig);
    return result;
}

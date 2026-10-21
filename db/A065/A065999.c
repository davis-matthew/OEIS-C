/*
 * A065999  Sum of digits of 9^n.
 * https://oeis.org/A065999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of decimal digits of 9^n
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* quick path for n == 0 */
    if (n == 0) return (seq_t)1;

    /* initial capacity: 1 digit + extra for growth */
    size_t capacity = (size_t)n + 4;
    if (capacity < 16) capacity = 16;

    uint8_t *digits = (uint8_t *)malloc(capacity);
    if (!digits) return (seq_t)0;          /* allocation failure => 0 */

    digits[0] = 1;                         /* 9^0 */
    size_t len = 1;                        /* number of decimal digits */

    for (idx_t iter = 0; iter < n; ++iter) /* compute 9^n iteratively */
    {
        uint8_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint16_t tmp = (uint16_t)digits[i] * 9 + carry; /* tmp <= 89 */
            digits[i] = (uint8_t)(tmp % 10);
            carry = (uint8_t)(tmp / 10);
        }
        if (carry)
        {
            /* ensure capacity for the new digit */
            if (len + 1 > capacity)
            {
                capacity *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, capacity);
                if (!tmp)
                {
                    free(digits);
                    return (seq_t)0;      /* allocation failure */
                }
                digits = tmp;
            }
            digits[len++] = carry;
        }
    }

    /* compute sum of digits */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)digits[i];

    free(digits);
    return sum;
}

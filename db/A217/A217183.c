/*
 * A217183  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains eight consecutive identical
 *          digits.
 * https://oeis.org/A217183
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digits in the smallest power of n that contains
 *            eight consecutive identical decimal digits
 * Range:     u8 none, u16 n <= 100, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return a(n) for n >= 2.  Works for any seq_t type.  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2 */

    /* base = n, used for multiplication */
    const size_t base = (size_t)n;

    /* digits[] stores the current value of n^k in reverse order
       (least significant decimal digit first).  */
    static uint8_t *digits = NULL;
    static size_t  cap    = 0;
    size_t len = 0;

    /* initialise digits with the decimal representation of n */
    size_t tmp = base;
    do
    {
        if (len == cap)
        {
            cap = cap ? cap * 2 : 2;
            uint8_t *tmpbuf = (uint8_t *)realloc(digits, cap);
            if (!tmpbuf) return 0;        /* allocation failure */
            digits = tmpbuf;
        }
        digits[len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);

    while (1)
    {
        /* look for eight consecutive equal digits */
        unsigned run = 1;
        for (size_t i = 1; i < len; ++i)
        {
            if (digits[i] == digits[i - 1])
            {
                if (++run >= 8) return (seq_t)len;
            }
            else
            {
                run = 1;
            }
        }

        /* multiply by base:  digit[i] = digit[i] * base + carry */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint64_t prod = (uint64_t)digits[i] * base + carry;
            digits[i] = prod % 10;
            carry     = prod / 10;
        }
        while (carry > 0)
        {
            if (len == cap)
            {
                cap = cap ? cap * 2 : 2;
                uint8_t *tmpbuf = (uint8_t *)realloc(digits, cap);
                if (!tmpbuf) return 0;    /* allocation failure */
                digits = tmpbuf;
            }
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }
}

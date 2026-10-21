/*
 * A055475  Powers of ten written in base 4.
 * https://oeis.org/A055475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal representation of the base‑4 digits of 10ⁿ
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0) return (seq_t)1;

    /* The number of base‑4 digits of 10ⁿ is ≤ 2·n + 10. */
    size_t cap = (size_t)(n) * 2 + 10;
    unsigned char *digits = (unsigned char*)malloc(cap);
    if (!digits) return (seq_t)0;          /* unlikely, but safe */

    size_t len = 1;      /* start with 10⁰ = 1 */
    digits[0] = 1;        /* base‑4 digit for 1 */

    /* Multiply successively by 10 decimal (22 in base‑4). */
    for (idx_t step = 1; step <= n; ++step)
    {
        uint32_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint32_t tmp = (uint32_t)digits[i] * 10u + carry;
            digits[i]   = (unsigned char)(tmp % 4u);   /* new digit */
            carry       = tmp / 4u;                    /* new carry */
        }
        while (carry > 0)
        {
            if (len == cap)
            {
                cap *= 2;
                unsigned char *tmp = (unsigned char*)realloc(digits, cap);
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
            }
            digits[len++] = (unsigned char)(carry % 4u);
            carry        /= 4u;
        }
    }

    /* Convert base‑4 digits (least significant first) to a decimal seq_t. */
    seq_t result = (seq_t)0;
    for (size_t i = len; i > 0; --i)
    {
        result = result * (seq_t)10 + (seq_t)digits[i - 1];
    }

    free(digits);
    return result;
}

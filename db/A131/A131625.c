/*
 * A131625  Numbers k such that decimal expansion of 3^k contains no 2.
 * https://oeis.org/A131625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th exponent k such that decimal expansion of 3^k contains no digit '2' */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array of decimal digits (least significant first). */
    size_t cap = 64;
    size_t sz  = 1;
    uint8_t *digits = (uint8_t*)malloc(cap);
    if (!digits) return (seq_t)0;          /* allocation failure */
    digits[0] = 1;                         /* 3^0 = 1 */

    idx_t  count = 0;      /* number of exponents found so far */
    idx_t  k     = 0;       /* current exponent, currently 0 */

    for (;;)
    {
        /* Check whether 3^k contains a digit 2. */
        bool has2 = false;
        for (size_t i = 0; i < sz; ++i)
        {
            if (digits[i] == 2)
            {
                has2 = true;
                break;
            }
        }

        if (!has2)
        {
            ++count;
            if (count == n)
            {
                seq_t result = (seq_t)k;
                free(digits);
                return result;
            }
        }

        /* Multiply the current decimal number by 3 to obtain 3^(k+1). */
        unsigned int carry = 0;
        for (size_t i = 0; i < sz; ++i)
        {
            unsigned int prod = 3 * digits[i] + carry;  /* prod ≤ 3*9+2 = 29 */
            digits[i] = prod % 10;
            carry    = prod / 10;
        }
        while (carry)
        {
            if (sz == cap)
            {
                cap <<= 1;                 /* double the capacity */
                uint8_t *tmp = (uint8_t*)realloc(digits, cap);
                if (!tmp)
                {
                    free(digits);
                    return (seq_t)0;
                }
                digits = tmp;
            }
            digits[sz++] = carry % 10;
            carry       /= 10;
        }
        ++k;  /* advance to next exponent */
    }
}

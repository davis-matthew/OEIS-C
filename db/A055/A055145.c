/*
 * A055145  The first n digits of the juxtaposition of the base-4 numbers
 *          converted to decimal.
 * https://oeis.org/A055145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal value of the first n digits of the infinite
 *            * base‑4 string obtained by concatenating the base‑4
 *            representations of the * positive integers in order
 *            (1,2,3,10,11,12,…). That is, * a(n) = ⎣(the n‑digit base‑4
 *            number formed from the first n digits of this * string)⎦_10. The
 *            func...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
/* For compilers that don't support C99, use ISO C90 macros */
#define _Static_assert(cond, msg) 
#endif

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for the sequence, but harmless */

    seq_t val = 0;
    idx_t needed = n;
    idx_t number = 1;

    /* temporary buffer for base‑4 digits of the current number */
    uint8_t digits[32];

    while (needed > 0)
    {
        /* convert 'number' to base‑4, least significant first */
        uint64_t tmp = (uint64_t)number;
        uint8_t len = 0;
        do {
            digits[len++] = tmp % 4;
            tmp /= 4;
        } while (tmp > 0);

        /* process digits most significant first */
        for (int i = len - 1; i >= 0 && needed > 0; --i)
        {
            val = val * 4 + digits[i];
            --needed;
        }
        ++number;
    }

    return val;
}

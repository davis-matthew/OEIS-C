/*
 * A171593  a(n) = 0+1+2+...+n in lunar arithmetic in base 6 written in base
 *          6.
 * https://oeis.org/A171593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=0}^{n} k in lunar arithmetic base 6, written in
 *            base 6
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Store the lunar‑sum digits (base 6) in reverse order.  */
    seq_t digits[64];
    size_t len = 0;

    idx_t power = 1;            /* 6^k  */
    idx_t remaining = n;        /* digits of n that are still unprocessed */

    /*  For each position: if n ≥ 5·6^k then the digit is 5,
        otherwise it is the corresponding digit of n.  */
    while (remaining > 0 || (n / power >= 5))
    {
        seq_t digit;
        if (n / power >= 5)
            digit = 5;
        else
            digit = remaining % 6;   /* current 6‑ary digit of n */

        digits[len++] = digit;
        remaining /= 6;
        power *= 6;
    }

    /*  Convert the collected base‑6 digits to the decimal integer
        that represents those digits.  */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i)
        res = res * 10 + digits[len - 1 - i];

    return res;
}

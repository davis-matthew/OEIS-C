/*
 * A095540  a(n) = 74 written in base n.
 * https://oeis.org/A095540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal number obtained by writing 74 in base n * and
 *            concatenating the decimal representations of the * base-n digits
 *            (one digit may be more than 9)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Handle the special unary case n == 1 */
    if (n == 1) {
        seq_t repunit = 0;
        for (idx_t i = 0; i < 74; ++i)
            repunit = repunit * 10 + 1;
        return repunit;
    }

    /* Extract the digits of 74 in base n.  The number 74 is
     * small, so the array size 64 is more than enough. */
    seq_t digits[64];
    size_t d = 0;
    idx_t t = 74;
    while (t > 0) {
        digits[d++] = (seq_t)(t % n);
        t /= n;
    }

    /* Convert the array of digits to the required decimal number. */
    seq_t result = 0;
    for (size_t i = d; i-- > 0; ) {
        seq_t di = digits[i];
        idx_t k   = (idx_t)digit_len(di);      /* number of decimal digits of di */
        seq_t pow = pow10(k);                  /* 10^k  */
        result = result * pow + di;
    }
    return result;
}

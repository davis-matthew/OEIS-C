/*
 * A095468  a(n) = 38 written in base n.
 * https://oeis.org/A095468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal concatenation of the base‑n digits of 38 *
 *            Each base‑n digit d is written in decimal with no separators, so
 *            * digits >9 are represented by their full decimal value (e.g. 12
 *            → “12”). * For n = 1 the representation is 38 copies of the
 *            digit ‘1’. * For n > 38 the representation is...
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 12, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle offsets below the series start */
    if (n == 0) return 0;

    /* Base‑1: 38 ones */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 38; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* For bases larger than 38 the representation is a single digit 38 */
    if (n > 38) return 38;

    /* Compute the digits of 38 in base n */
    idx_t digits[64];
    idx_t cnt = 0;
    idx_t temp = 38;
    while (temp > 0) {
        digits[cnt++] = temp % n;
        temp /= n;
    }

    /* Convert the digits to a decimal integer by concatenating their
     * decimal representations (no separators).  Digits may be up to
     * 9999, so at most 4 decimal digits.  */
    seq_t res = 0;
    for (idx_t j = 0; j < cnt; ++j) {
        seq_t d = (seq_t)digits[cnt - 1 - j];
        seq_t pow10_len;
        if (d < 10)          pow10_len = 10;
        else if (d < 100)    pow10_len = 100;
        else if (d < 1000)   pow10_len = 1000;
        else                 pow10_len = 10000;
        res = res * pow10_len + d;
    }
    return res;
}

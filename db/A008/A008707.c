/*
 * A008707  a(n) = 10 in base 10-n.
 * https://oeis.org/A008707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal representation of 10 in base (10-n)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute base = 10 - n */
    int64_t base = 10 - (int64_t)n;

    /* base >= 2 : normal positional system */
    if (base >= 2) {
        seq_t num = 10;              /* decimal 10 */
        int  digits[16];
        int  dlen = 0;

        /* get base representation digits (least significant first) */
        while (num > 0) {
            seq_t rem = num % (seq_t)base;
            num /= (seq_t)base;
            digits[dlen++] = (int)rem;
        }

        /* build the decimal number from the reversed digits */
        seq_t res = 0;
        for (int i = dlen - 1; i >= 0; --i)
            res = res * 10 + (seq_t)digits[i];

        return res;
    }

    /* base == 1 : unary representation -> ten 1's */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 10; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* base <= 0 : undefined – return 0 */
    return 0;
}

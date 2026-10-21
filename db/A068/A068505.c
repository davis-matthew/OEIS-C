/*
 * A068505  Decimal representation of n interpreted in base b+1, where
 *          b=A054055(n) is the largest digit in decimal representation of n.
 * https://oeis.org/A068505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = interpret decimal digits of n in base (max decimal digit
 *            + 1)
 * Range:     u8 n <= 258, u16 n <= 9531, u32 n <= 9531, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n should be >= 1 for the sequence; 0 is handled as a fallback */
    if (n == 0) {
        return (seq_t)0;
    }

    unsigned char digits[20];  /* 20 digits are enough for 64‑bit numbers */
    size_t len = 0;
    unsigned maxdigit = 0;
    idx_t tmp = n;

    /* Extract decimal digits (least significant first) and find the maximum digit */
    while (tmp != 0) {
        unsigned d = (unsigned)(tmp % 10);
        digits[len++] = (unsigned char)d;
        if (d > maxdigit) {
            maxdigit = d;
        }
        tmp /= 10;
    }

    seq_t base = (seq_t)(maxdigit + 1);
    seq_t res = 0;

    /* Interpret the digits as a number in the obtained base,
       performing the calculation from most significant to least. */
    for (size_t i = len; i-- > 0;) {
        res = res * base + digits[i];
    }

    return res;
}

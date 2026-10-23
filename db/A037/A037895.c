/*
 * A037895  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*9^i} is the base 9
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*Σ|d(i)-e(i)| where Σd(i)*9^i = n in base 9 and
 *            e(i)=d(k-1-i)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑9 digits (least‑significant first) */
    uint8_t digits[32];   /* 32 digits is ample for any 64‑bit n in base 9 */
    size_t len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 9U);
            tmp /= 9U;
        }
    }

    /* Sum absolute differences of mirrored digits */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        uint8_t e = digits[len - 1 - i];
        uint8_t diff = d > e ? d - e : e - d;
        sum += diff;
    }

    /* The sum is always even, divide by 2 to obtain the final value */
    return sum / 2;
}

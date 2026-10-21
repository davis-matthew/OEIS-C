/*
 * A222233  In the number n, replace all (decimal) digits '2' with '9' and
 *          vice versa.
 * https://oeis.org/A222233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each decimal digit 2 with 9 and 9 with 2 in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    uint8_t digits[20];
    int d = 0;
    uint64_t temp = n;
    /* Extract decimal digits of n into digits[] in reverse order */
    while (temp > 0) {
        digits[d++] = temp % 10;
        temp /= 10;
    }
    /* Build the result from most significant to least, applying the swap */
    for (int i = d - 1; i >= 0; --i) {
        uint8_t dig = digits[i];
        if (dig == 2) dig = 9;
        else if (dig == 9) dig = 2;
        res = res * 10 + dig;
    }
    return res;
}

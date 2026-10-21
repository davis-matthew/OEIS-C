/*
 * A053392  a(n) is the concatenation of the sums of every pair of consecutive
 *          digits of n (with a(n) = 0 for 0 <= n <= 9).
 * https://oeis.org/A053392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base, look
 * Formula:   a(n) = concatenation of the sums of every pair of consecutive
 *            decimal digits of n; a(n) = 0 for 0 <= n <= 9
 * Range:     u8 n <= 127, u16 n <= 1645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For single-digit n the value is 0. */
    if (n < 10) {
        return 0;
    }

    /* Extract decimal digits of n into an array (least significant first). */
    uint8_t digits[20];   /* Max 20 digits for 64‑bit unsigned integer. */
    int len = 0;
    while (n > 0) {
        digits[len++] = n % 10;
        n /= 10;
    }

    seq_t result = 0;

    /* Process pairs from most significant to least significant. */
    for (int i = len - 1; i >= 1; --i) {
        int sum = digits[i] + digits[i - 1];
        if (sum < 10) {
            result = result * 10 + sum;
        } else { /* sum is 10..18, two decimal digits */
            result = result * 100 + sum;
        }
    }

    return result;
}

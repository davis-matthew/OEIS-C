/*
 * A063661  a(n) = A063660(n) - n.
 * https://oeis.org/A063661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, base, look
 * Formula:   a(n) = A063660(n) - n For n < 10: a(n) = 10 - [n = 1]. For n >=
 *            10: if n has a leading digit k (k ≠ 8) followed only by 9’s,
 *            then a(n) = k + 1; otherwise a(n) = 1
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 10) {
        /* Handle the initial 10 terms */
        return (seq_t)(n == 1 ? 9 : 10);
    }

    /* Extract decimal digits of n */
    uint8_t digits[20];          /* max 20 digits for uint64_t */
    int len = 0;
    idx_t temp = n;
    do {
        digits[len++] = (uint8_t)(temp % 10);
        temp /= 10;
    } while (temp);

    uint8_t first = digits[len - 1];  /* most significant digit */
    bool all9 = true;
    for (int i = 0; i < len - 1; ++i) {
        if (digits[i] != 9) {
            all9 = false;
            break;
        }
    }

    if (all9 && first != 8) {
        return (seq_t)(first + 1);
    }
    return (seq_t)1;
}

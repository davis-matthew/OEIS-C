/*
 * A028907  Arrange digits of squares in ascending order.
 * https://oeis.org/A028907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   the sequence is defined by sorting the decimal digits of n² in
 *            ascending order and discarding any leading zeros. Thus a(n) =
 *            ascending_digit_sort(n²), with leading zeros removed
 * Range:     u8 n <= 15, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    uint8_t buf[128];   /* buffer for the digits of n²; 128 is more than enough */
    size_t len;

    get_digits(sq, buf, &len);          /* buf now holds the digits of n² */

    unsigned cnt[10] = {0};             /* count of each digit 0‑9 */
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        ++cnt[d];
    }

    seq_t result = 0;
    int leading_zero = 1;               /* flag to skip initial zeros */

    for (int d = 0; d <= 9; ++d) {
        unsigned k = cnt[d];
        while (k--) {
            if (leading_zero && d == 0) {
                continue;               /* skip leading zeros */
            }
            leading_zero = 0;
            result = result * 10 + d;
        }
    }
    return result;
}

/*
 * A079050  Sum of the digits of LookAndSay(n).
 * https://oeis.org/A079050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of the look‑and‑say expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sum_digits_uint64.h"

seq_t a(idx_t n)
{
    /* Convert n to its decimal digits (least significant first). */
    uint8_t digits[20];
    size_t len = 0;
    uint64_t t = (uint64_t)n;

    if (t == 0) {
        digits[len++] = 0;
    } else {
        while (t > 0) {
            digits[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    seq_t sum = 0;
    size_t i = len;
    /* Scan runs of equal digits from most significant to least. */
    while (i > 0) {
        i--;                            /* index of current digit (from the MS side). */
        uint8_t d = digits[i];
        uint64_t count = 1;
        while (i > 0 && digits[i - 1] == d) {
            i--;
            count++;
        }
        sum += sum_digits_uint64(count) + (seq_t)d;
    }
    return sum;
}

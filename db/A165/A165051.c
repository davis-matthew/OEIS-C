/*
 * A165051  a(n) = image of n under the base-6 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n sorted descending) - (digits of n sorted
 *            ascending) in base 6
 * Range:     u8 n <= 217, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base‑6 digit array (max 30 digits for n < 2^64) */
    uint8_t buf[30];
    uint8_t asc[30];
    idx_t len = 0;

    /* Special case: n = 0 */
    if (n == 0) {
        asc[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        /* Extract digits in little‑endian order */
        while (t > 0) {
            buf[len++] = t % 6;
            t /= 6;
        }

        /* Sort the digits ascending using insertion sort */
        for (idx_t i = 1; i < len; ++i) {
            uint8_t key = buf[i];
            int j = (int)i - 1;
            while (j >= 0 && buf[j] > key) {
                buf[j + 1] = buf[j];
                --j;
            }
            buf[j + 1] = key;
        }

        /* Copy to ascending array */
        for (idx_t i = 0; i < len; ++i) asc[i] = buf[i];
    }

    /* Build smallest number (ascending order) */
    seq_t small = 0;
    for (idx_t i = 0; i < len; ++i) {
        small = small * 6 + asc[i];
    }

    /* Build largest number (descending order) */
    seq_t large = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        large = large * 6 + asc[i];
    }

    /* Return difference */
    return large - small;
}

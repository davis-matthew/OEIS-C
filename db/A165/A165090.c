/*
 * A165090  a(n) is the image of n under the base-8 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n in base‑8 sorted descending) - (digits of n
 *            in base‑8 sorted ascending)
 * Range:     u8 n <= 68, u16 n <= 9666, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t build_from_digits(const uint8_t *digits, size_t len)
{
    seq_t v = 0;
    for (size_t i = 0; i < len; ++i)
        v = v * 8 + digits[i];
    return v;
}

seq_t a(idx_t n)
{
    /* up to 22 octal digits for uint64_t values – small fixed array */
    uint8_t raw[22];
    uint8_t asc[22];
    size_t len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        raw[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            raw[len++] = (uint8_t)(tmp & 7);   /* tmp % 8 */
            tmp >>= 3;                         /* tmp / 8 */
        }
    }

    /* copy raw to asc for sorting */
    for (size_t i = 0; i < len; ++i)
        asc[i] = raw[i];

    /* simple insertion sort on asc (ascending order) */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = asc[i];
        int j = (int)i - 1;
        while (j >= 0 && asc[j] > key) {
            asc[j + 1] = asc[j];
            --j;
        }
        asc[j + 1] = key;
    }

    /* build ascending value (most significant first) */
    seq_t asc_val = build_from_digits(asc, len);

    /* build descending value by traversing asc from the end */
    seq_t desc_val = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        desc_val = desc_val * 8 + asc[i];

    return desc_val - asc_val;
}

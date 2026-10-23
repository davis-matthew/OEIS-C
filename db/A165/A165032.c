/*
 * A165032  a(n) = image of n under the base-5 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-5 Kaprekar map difference: a(n) = (digits of n sorted descending in base 5) − (digits of n sorted ascending in base 5) */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* Extract base‑5 digits (least significant first) */
    uint8_t digits[64];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 5;
        t /= 5;
    }

    /* Count digits to sort them in ascending/descending order */
    int cnt[5] = {0};
    for (size_t i = 0; i < len; ++i)
        ++cnt[digits[i]];

    /* Build ascending order array */
    uint8_t asc[64];
    size_t pos = 0;
    for (int d = 0; d < 5; ++d)
        for (int c = 0; c < cnt[d]; ++c)
            asc[pos++] = (uint8_t)d;

    /* Build descending order array (reverse of ascending) */
    uint8_t desc[64];
    pos = 0;
    for (int d = 4; d >= 0; --d)
        for (int c = 0; c < cnt[d]; ++c)
            desc[pos++] = (uint8_t)d;

    /* Compute numeric values of both orderings in base 5 */
    seq_t val_asc = 0;
    for (size_t i = 0; i < len; ++i)
        val_asc = val_asc * (seq_t)5 + asc[i];

    seq_t val_desc = 0;
    for (size_t i = 0; i < len; ++i)
        val_desc = val_desc * (seq_t)5 + desc[i];

    return val_desc - val_asc;
}

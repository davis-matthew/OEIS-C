/*
 * A165071  a(n) = image of n under the base-7 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n in base‑7 sorted descending) - * (digits of
 *            n in base‑7 sorted ascending)
 * Range:     u8 n <= 54, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n == 0 gives both sorted numbers equal to 0 */
    if (n == 0)
        return 0;

    /* Count occurrences of each base‑7 digit (0..6) in n */
    unsigned char cnt[7] = {0};
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned digit = tmp % 7;
        ++cnt[digit];
        tmp /= 7;
    }

    /* Build the number with digits in descending order */
    seq_t desc = 0;
    for (int d = 6; d >= 0; --d)
        for (unsigned k = 0; k < cnt[d]; ++k)
            desc = desc * 7 + d;

    /* Build the number with digits in ascending order */
    seq_t asc = 0;
    for (int d = 0; d <= 6; ++d)
        for (unsigned k = 0; k < cnt[d]; ++k)
            asc = asc * 7 + d;

    /* Difference (non‑negative) */
    return desc - asc;
}

/*
 * A164993  a(n) = image of n under the base-3 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A164993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n sorted descending) - (digits of n sorted
 *            ascending) in base‑3 Kaprekar map
 * Range:     u8 n <= 243, u16 n <= 9259, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t temp = n;             /* work on a copy of n */
    uint8_t cnt[3] = {0, 0, 0};  /* count of digits 0,1,2 */

    /* separate digits of n in base 3 */
    while (temp > 0) {
        seq_t rem = temp % 3;
        cnt[(int)rem]++;
        temp /= 3;
    }

    seq_t asc = 0;   /* number built from ascending digits */
    seq_t desc = 0;  /* number built from descending digits */

    /* construct ascending number */
    for (int d = 0; d <= 2; ++d)
        for (uint8_t i = 0; i < cnt[d]; ++i)
            asc = asc * 3 + d;

    /* construct descending number */
    for (int d = 2; d >= 0; --d)
        for (uint8_t i = 0; i < cnt[d]; ++i)
            desc = desc * 3 + d;

    return desc - asc;
}

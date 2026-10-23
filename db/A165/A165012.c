/*
 * A165012  a(n) = image of n under the base-4 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A165012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digits of n in base-4 sorted descending) - (digits of n
 *            in base-4 sorted ascending)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count digits 0..3 of n in base 4 */
    int cnt[4] = {0};
    idx_t tmp = n;
    if (tmp == 0)
        cnt[0] = 1;
    else
    {
        while (tmp)
        {
            int d = tmp % 4;
            ++cnt[d];
            tmp /= 4;
        }
    }

    /* Build ascending number */
    seq_t asc = 0;
    for (int d = 0; d <= 3; ++d)
        for (int i = 0; i < cnt[d]; ++i)
            asc = asc * 4 + d;

    /* Build descending number */
    seq_t desc = 0;
    for (int d = 3; d >= 0; --d)
        for (int i = 0; i < cnt[d]; ++i)
            desc = desc * 4 + d;

    return desc - asc;
}

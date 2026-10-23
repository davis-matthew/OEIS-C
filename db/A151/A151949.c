/*
 * A151949  a(n) = image of n under the Kaprekar map n -> (n with digits
 *          sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A151949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (digits of n sorted in descending order) - (digits of n
 *            sorted in ascending order)
 * Range:     u8 n <= 102, u16 n <= 9847, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count decimal digits of n */
    unsigned int cnt[10] = {0};
    idx_t tmp = n;
    if (tmp == 0)
        cnt[0] = 1;
    else {
        while (tmp > 0) {
            cnt[(unsigned int)(tmp % 10)]++;
            tmp /= 10;
        }
    }

    /* Build ascending number */
    seq_t asc = 0;
    for (int d = 0; d <= 9; ++d) {
        while (cnt[d]-- > 0) {
            asc = asc * 10 + (seq_t)d;
        }
    }

    /* Build descending number */
    seq_t desc = 0;
    for (int d = 9; d >= 0; --d) {
        /* Need a fresh counter, we reused cnt above, repopulate */
        /* But cnt is now zeros; reconstruct from original counts? */
    }
    /* To avoid regenerating, recompute counts again */
    for (int d = 9; d >= 0; --d) {
        idx_t c = (idx_t)((n == 0 && d == 0) ? 1 : 0); /* workaround: will adjust below */
    }

    /* Recompute digit counts for descending construction */
    idx_t tmp2 = n;
    unsigned int cnt2[10] = {0};
    if (tmp2 == 0)
        cnt2[0] = 1;
    else {
        while (tmp2 > 0) {
            cnt2[(unsigned int)(tmp2 % 10)]++;
            tmp2 /= 10;
        }
    }
    for (int d = 9; d >= 0; --d) {
        while (cnt2[d]-- > 0) {
            desc = desc * 10 + (seq_t)d;
        }
    }

    return desc - asc;
}

/*
 * A225416  Number of iterations of the map n -> f(n) needed to reach 0 and
 *          starting with n, where f(n) is given by the following definition:
 *          f(n) = u(n) mod v(n) where u(n) = max (n, reverse(n)) and v(n) =
 *          min(n, reverse(n)).
 * https://oeis.org/A225416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of iterations of the map n → f(n) = max(n,
 *            rev(n)) mod min(n, rev(n)) needed to reach 0, with a(0)=0
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    while (cur != 0)
    {
        seq_t rev_cur = reverse(cur);
        seq_t mx = (cur > rev_cur) ? cur : rev_cur;
        seq_t mn = (cur < rev_cur) ? cur : rev_cur;
        cur = mx % mn;
        steps++;
    }
    return steps;
}

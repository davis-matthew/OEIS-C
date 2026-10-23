/*
 * A029825  Average theta series of odd unimodular lattices of dimension 24
 *          (multiplied by 691).
 * https://oeis.org/A029825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * Σ_{d|n} d^11 - 32 * Σ_{odd d|n, 2d|n} d^11, a(0)=691
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow11(seq_t x)
{
    seq_t r = 1;
    for (int i = 0; i < 11; ++i)
        r *= x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)691;

    seq_t sum1 = 0;
    seq_t sum2 = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t d2 = n / d;

        seq_t pd = pow11((seq_t)d);
        sum1 += pd;
        if ((d & 1) && !(d2 & 1))   /* odd d and n/d even → 2d|n */
            sum2 += pd;

        if (d2 != d)
        {
            seq_t pd2 = pow11((seq_t)d2);
            sum1 += pd2;
            if ((d2 & 1) && !(d & 1))   /* odd d2 and n/d2 even → 2d2|n */
                sum2 += pd2;
        }
    }

    return sum1 * (seq_t)16 - sum2 * (seq_t)32;
}

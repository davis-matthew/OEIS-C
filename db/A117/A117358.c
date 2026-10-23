/*
 * A117358  a(n) = A032742(A032742(A032742(n))) =
 *          ((n/lpf(n))/lpf(n/lpf(n)))/lpf((n/lpf(n))/lpf(n/lpf(n))), where
 *          lpf=A020639, least prime factor.
 * https://oeis.org/A117358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A032742(A032742(A032742(n))) =
 *            ((n/lpf(n))/lpf(n/lpf(n)))/lpf((n/lpf(n))/lpf(n/lpf(n)))
 * Range:     u8 n <= 2047, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    idx_t m = n;
    for (int i = 0; i < 3; ++i)
    {
        if (m <= 1) break;
        idx_t l = lpf(m);
        if (l <= 1) break;
        m /= l;
    }
    return (seq_t)m;
}

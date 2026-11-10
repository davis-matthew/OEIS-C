/*
 * A059649  Positions of ones in A059648.
 * https://oeis.org/A059649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with (2k-1)^2 >= 2*m^2, where m =
 *            floor(sqrt(2)*k), i.e. the k with A059648(k)=1; found by a
 *            linear scan
 * Range:     u8 n <= 37, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t cnt = 0;
    seq_t k = 0;
    seq_t m = 0; /* floor(sqrt(2)*k) */
    while (1)
    {
        k++;
        m++;
        /* floor(sqrt(2)*k) increases by 1 or 2 per step */
        if ((m + 1) * (m + 1) <= (seq_t)2 * k * k)
            m++;
        seq_t t = (k << 1) - 1;
        if (t * t >= (seq_t)2 * m * m)
        {
            cnt++;
            if (cnt == target)
                return k;
        }
    }
}

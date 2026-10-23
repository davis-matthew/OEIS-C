/*
 * A124870  Denominator of real part of (3*i - 1)^(-n).
 * https://oeis.org/A124870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2500*a(n-4) for n > 6, with a(0)=1, a(1)=10, a(2)=25,
 *            a(3)=500, a(4)=2500, a(5)=25000, a(6)=31250
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[7] = { (seq_t)1, (seq_t)10, (seq_t)25,
                                   (seq_t)500, (seq_t)2500,
                                   (seq_t)25000, (seq_t)31250 };

    if (n <= 6)
        return init[(size_t)n];

    /* Number of times to multiply by 2500: ceil((n-6)/4) == (n-3)/4 for n>6 */
    idx_t m = (n - 3) / 4;
    idx_t r = n - 4 * m;          /* r is guaranteed ≤ 6 */
    seq_t res = init[(size_t)r];

    for (idx_t i = 0; i < m; ++i)
        res *= (seq_t)2500;

    return res;
}

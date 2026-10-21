/*
 * A054600  Sum_{d|n, d odd} d*2^(n/d).
 * https://oeis.org/A054600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{d | n , d odd} d * 2^(n/d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d != 0)
            continue;

        if (d & 1) /* odd divisor d */
        {
            seq_t term = (seq_t)d * pow2(n / d);
            sum += term;
        }

        idx_t e = n / d;        /* paired divisor */
        if (e != d && (e & 1))  /* odd and not same as d */
        {
            seq_t term = (seq_t)e * pow2(n / e);
            sum += term;
        }
    }

    return sum;
}

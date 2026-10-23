/*
 * A159605  E.g.f: Sum_{n>=1} a(n)*x^(2n-1)/(2n-1)! = Series_Reversion of
 *          e.g.f. S(x) of A159601.
 * https://oeis.org/A159605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2k-3)(4k-5)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Start from 1 as the product is empty for n=0 (not used) */
    seq_t result = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* use signed intermediate to avoid underflow when k=1 */
        int64_t k_s   = (int64_t)k;
        int64_t mult  = (2 * k_s - 3) * (4 * k_s - 5);
        result *= (seq_t)mult;
    }

    return result;
}

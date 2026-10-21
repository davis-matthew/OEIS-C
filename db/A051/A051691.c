/*
 * A051691  a(n) = (5*n+10)(!^5)/10(!^5), related to A052562 ((5*n)(!^5)
 *          quintic, or 5-factorials).
 * https://oeis.org/A051691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n · (n+2)! / 2 Equivalently a(0) = 1 and a(n) = a(n-1)
 *            * (5*(n+2)) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute a(n) directly as the product of the first n terms
     * (5*(k+2)) for k = 1 … n.  For n = 0 the empty product gives 1.   */
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        result *= (seq_t)(5 * (k + 2));
    }
    return result;
}

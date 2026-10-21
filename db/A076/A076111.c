/*
 * A076111  Product of terms in n-th row of A076110.
 * https://oeis.org/A076111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Prod_{k=1..n} (1 + n*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = 1 + n*k   (cast to seq_t before multiplication) */
        res *= (seq_t)(1 + n * k);
    }
    return res;
}

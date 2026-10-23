/*
 * A138586  a(1) = 1; a(n) = a(n-1) + (n!)^7.
 * https://oeis.org/A138586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k!)^7
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum  = 0;
    seq_t fact = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        fact = mul_small(fact, (unsigned int)k);   /* fact *= k */
        seq_t term = pow_seq(fact, 7);              /* (k!)^7 */
        sum += term;
    }
    return sum;
}

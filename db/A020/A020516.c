/*
 * A020516  Sum of n-th powers of divisors of 64.
 * https://oeis.org/A020516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^n + 2^(2n) + … + 2^(6n) (sum of n‑th powers of
 *            divisors of 64)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* base = 2^n */
    seq_t base = pow2(n);
    seq_t term = 1;        /* term = 2^(k*n) starting with k=0 */
    seq_t sum  = 0;

    for (idx_t i = 0; i < 7; ++i) {
        sum += term;      /* add current term to sum        */
        term *= base;     /* advance to next power of 2^n   */
    }
    return sum;
}

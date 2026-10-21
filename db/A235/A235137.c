/*
 * A235137  a(n) = Sum_{k = 1..n} k^phi(n), where phi(n) = A000010(n).
 * https://oeis.org/A235137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 1..n} k^phi(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* sequence is defined for n ≥ 1 */
        return 0;

    seq_t phi_n = phi(n);        /* φ(n) as seq_t */
    seq_t total = 0;             /* accumulator for the sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        total += big_pow((seq_t)k, phi_n);   /* add k^φ(n) */
    }
    return total;
}

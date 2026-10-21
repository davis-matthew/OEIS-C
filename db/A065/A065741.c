/*
 * A065741  Largest square <= sum of squares of divisors of n.
 * https://oeis.org/A065741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt( Σ_{d|n} d² ) )²
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt_seq_local, sigma2_divisor_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest square ≤ the sum of squares of divisors of n. */

#include <stdint.h>
#include "../utils/floor_sqrt_seq_local.h"
#include "../utils/sigma2_divisor_square_sum.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1; behaviour for n<1 is unused */
    seq_t sig2 = sigma2_divisor_square_sum(n);
    seq_t r = floor_sqrt_seq_local(sig2);
    return r * r;
}

/*
 * A035207  Coefficients in expansion of Dirichlet series Product_p (1 -
 *          (Kronecker(m,p) + 1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          25.
 * https://oeis.org/A035207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = number of divisors of n that are not divisible by 5,
 *            equivalently a(n) = τ(n / 5^k), where k = vp5(n) is the largest
 *            power of 5 dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9056 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    idx_t x = n;
    while (x % 5 == 0)
        x /= 5;
    return num_divisors(x);
}

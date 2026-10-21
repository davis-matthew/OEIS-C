/*
 * A240667  a(n) is the GCD of the solutions x of sigma(x) = n; sigma(n) =
 *          A000203(n) = sum of divisors of n.
 * https://oeis.org/A240667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd{ x | sigma(x) = n }, 0 if no such x exists
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 0;                 /* 0 ⇔ no solution yet */
    for (idx_t x = 1; x <= n; ++x) {   /* x can be at most n (sigma(x) ≥ x) */
        seq_t sx = sigma_of(x);        /* sigma(x) */
        if (sx == n) {                 /* x is a solution */
            if (result == 0) result = x;
            else result = gcd(result, x);
        }
    }
    return result;
}

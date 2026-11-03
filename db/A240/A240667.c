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
 * Validated: standard, pass, 98 terms through n = 9990 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

/*
 * A088218  Total number of leaves in all rooted ordered trees with n edges.
 * https://oeis.org/A088218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n - 1, n) (OEIS A088218)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Computes the binomial coefficient  (2n-1 choose n) safely for all
// idx_t and seq_t types using only integer arithmetic while avoiding
// overflow in intermediate values.

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;            /* a(0) = 1 by convention */

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t num = (seq_t)(2 * (seq_t)n - (seq_t)i);   /* numerator = 2n - i */
        seq_t den = i;                                 /* denominator = i */

        /* Reduce numerator and denominator by their gcd */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce current result by gcd with remaining denominator */
        g = gcd(result, den);
        result /= g;
        den /= g;

        /* Now den should be 1 (numerical invariants guarantee this) */
        result *= num;
    }

    return result;
}

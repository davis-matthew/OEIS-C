/*
 * A017759  Binomial coefficients C(n,95).
 * https://oeis.org/A017759
 *
 * Offset:    95    (a(95) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,95) (binomial coefficient)
 * Range:     u8 n <= 96, u16 n <= 97, u32 n <= 101, u64 n <= 111,
 *            u128 n <= 143, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 95. For other n we return 0. */
    if (n < 95) return 0;

    const idx_t k = 95;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        /* numerator = n - k + i */
        seq_t num = n - k + i;

        /* Reduce the division at each step to keep intermediate values minimal. */
        seq_t g = gcd(result, (seq_t)i);
        result /= g;
        seq_t den = i / g;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        result *= num;
        result /= den;
    }

    return result;
}

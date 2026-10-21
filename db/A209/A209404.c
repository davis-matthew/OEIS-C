/*
 * A209404  Negated coefficients of Chebyshev T polynomials: a(n) =
 *          -A053120(n+14, n), n >= 0.
 * https://oeis.org/A209404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*binomial(n+6,6)*(n+14)/7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 39, u128 n <= 94,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 1 (special case because 2^(n-1) is not defined for n=0) */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Compute binomial coefficient (n+6 choose 6) */
    seq_t bin = binom(n + 6, 6);

    /* (n+14) as seq_t */
    seq_t mult = (seq_t)(n + 14);

    /* Divide by 7 as early as possible to keep the intermediate
       product small and avoid overflow on 64‑bit builds. */
    seq_t y;
    if (bin % (seq_t)7 == 0)
        y = (bin / (seq_t)7) * mult;
    else
        y = bin * (mult / (seq_t)7);

    /* 2^(n-1) */
    seq_t pow = pow2(n - 1);

    /* Final multiplication */
    return y * pow;
}

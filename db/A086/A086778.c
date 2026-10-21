/*
 * A086778  Number of n X n matrices over GF(5) with rank n-1.
 * https://oeis.org/A086778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ∏_{j=0}^{n-2} (5ⁿ−5ʲ)² / (5ⁿ⁻¹−5ʲ) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* the offset: a(1) = 1 */
    if (n == 1) return (seq_t)1;

    /* compute 5ⁿ and 5ⁿ⁻¹ */
    seq_t pow5_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) pow5_n *= (seq_t)5;
    seq_t pow5_n_minus1 = pow5_n / (seq_t)5;

    /* numerator and denominator of the (partial) product */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    /* current power 5ʲ, starts at 5⁰ = 1 */
    seq_t pow5_j = (seq_t)1;

    for (idx_t j = 0; j < n - 1; ++j)   /* j = 0 .. n‑2 */
    {
        seq_t val   = pow5_n - pow5_j;          /* 5ⁿ − 5ʲ   */
        seq_t denom = pow5_n_minus1 - pow5_j;    /* 5ⁿ⁻¹ − 5ʲ */

        /* cancel a common factor of current num and the new denominator */
        seq_t g = gcd(num, denom);
        if (g > 1)
        {
            num   /= g;
            denom /= g;
        }

        /* multiply the numerator by val twice (the square) */
        num *= val;
        num *= val;

        /* multiply the denominator */
        den *= denom;

        /* reduce the fraction again to keep the numbers small */
        g = gcd(num, den);
        if (g > 1)
        {
            num /= g;
            den /= g;
        }

        /* advance pow5_j for the next j */
        pow5_j *= (seq_t)5;
    }

    /* The final result is an integer: denominator should be 1 */
    return num;
}

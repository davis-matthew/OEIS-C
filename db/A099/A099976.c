/*
 * A099976  Bisection of A000984.
 * https://oeis.org/A099976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n+2, 2*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute a(n) = C(4n+2, 2n+1) using a multiplicative algorithm
   that cancels factors at each step.  The intermediate
   product result * num never exceeds the final value, so the
   computation fits in the type seq_t whenever the answer fits.  */
seq_t a(idx_t n)
{
    idx_t N = 4 * n + 2;          /* upper parameter of the binomial */
    idx_t K = 2 * n + 1;          /* lower parameter of the binomial */
    seq_t result = 1;             /* result variable in seq_t                  */

    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i);   /* numerator (N-K+i)           */
        seq_t den = (seq_t)i;             /* denominator i               */

        /* Cancel common factors between result and den */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* Cancel remaining common factors between num and den */
        g = gcd(num, den);
        num /= g;
        den /= g;   /* den should now be 1 */

        /* Multiply the remaining factor into the result */
        result *= num;
        /* (den is 1 so nothing remains to divide) */
    }
    return result;
}

/*
 * A166813  Number of n X 8 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) - 2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 55, u64 n <= 959,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t binom_n_plus_8_choose_8(idx_t n)
{
    seq_t N = ((seq_t)n) + 8;          /* N = n + 8 */
    seq_t res = 1;                     /* res will hold the binomial value */

    /* Compute C(N,8) using a safe multiplicative method.
     * At each step we cancel common factors with the current result
     * and with the numerator to keep the intermediate product
     * in the range of seq_t. */
    for (idx_t i = 1; i <= 8; ++i) {
        seq_t num   = N - 8 + i;       /* (N - 8 + i) = (N - (8-i))   */
        seq_t denom = i;               /* denominator = i             */

        seq_t g = gcd(res, denom);     /* cancel factors common to res and denom */
        res    /= g;
        denom  /= g;

        g = gcd(num, denom);           /* cancel remaining factors from num */
        num   /= g;
        denom /= g;                    /* denom should now be 1          */

        res *= num;                    /* safe: res*num <= final result */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 return 0 (unused). */
    if (n == 0) return 0;

    seq_t val = binom_n_plus_8_choose_8(n);
    val = val - 2;                     /* a(n) = C(n+8,8) - 2          */
    return val;
}

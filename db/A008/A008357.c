/*
 * A008357  Coordination sequence for D_6 lattice.
 * https://oeis.org/A008357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (x^6+54*x^5+447*x^4+852*x^3+447*x^2+54*x+1)/(1-x)^6 *
 *            (the series of this rational function gives the coordination
 *            sequence for the D6 lattice)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial P(x) = x^6+54x^5+447x^4+852x^3+447x^2+54x+1 */
    static const idx_t coeff[7] = {
        1ULL, 54ULL, 447ULL, 852ULL, 447ULL, 54ULL, 1ULL
    };  /* coeff[k] is the coefficient of x^k in P(x) */

    seq_t result = 0;
    for (idx_t k = 0; k <= 6; ++k) {
        if (n < k)          /* binomial term is zero for n < k */
            continue;
        idx_t m = n - k + 5; /* argument for binomial: (n-k+5 choose 5) */
        seq_t term = binom(m, 5); /* binom returns seq_t */
        result += (seq_t)coeff[k] * term;
    }
    return result;
}

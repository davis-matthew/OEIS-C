/*
 * A035787  Coordination sequence for C_50 lattice.
 * https://oeis.org/A035787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^50
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Precompute the 51 binomial coefficients binom(50, j) once. */
    static seq_t bin50[51];
    static bool init = false;
    if (!init) {
        for (idx_t j = 0; j <= 50; ++j)
            bin50[j] = binomial(50, j);
        init = true;
    }

    /* two_n is the exponent we need to match in the series. */
    idx_t two_n = n + n;

    seq_t sum = 0;
    for (idx_t j = 0; j <= 50 && j <= two_n; ++j) {
        /* coefficient of x^(2n-j) in (1-x)^(-50) is binom(49+(2n-j),49) */
        seq_t term = bin50[j] * binomial(49 + (two_n - j), 49);
        sum += term;
    }
    return sum;
}

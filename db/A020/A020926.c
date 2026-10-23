/*
 * A020926  Expansion of 1/(1-4*x)^(15/2).
 * https://oeis.org/A020926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = binomial(2*n, n) * *
 *            ((2*n+13)*(2*n+11)*(2*n+9)*(2*n+7)*(2*n+5)*(2*n+3)*(2*n+1)) * /
 *            135135
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 22, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Binomial coefficient bin(2n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Denominator to be cancelled incrementally */
    seq_t denom = 135135;

    /* Cancel common factors between bin and denom first */
    seq_t g = gcd(bin, denom);
    bin   /= g;
    denom /= g;

    /* Product of the seven odd factors */
    seq_t prod = 1;
    for (idx_t j = 0; j < 7; ++j) {
        seq_t f = (seq_t)(2 * n + 1 + 2 * j);
        /* Cancel any remaining factors of denom from f */
        seq_t g2 = gcd(f, denom);
        f   /= g2;
        denom /= g2;
        prod *= f;
    }

    /* After the loop denom should be 1 (all factors cancelled) */
    /* Final value: product * bin / (original denom) */
    seq_t result = prod * bin;
    /* Denom should be 1 – safety check */
    if (denom != (seq_t)1) {
        result /= denom;
    }

    return result;
}

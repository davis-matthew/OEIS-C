/*
 * A035785  Coordination sequence for C_48 lattice.
 * https://oeis.org/A035785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^48
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the coefficient of x^(2n) in ((1+x)/(1-x))^48.
 * This equals sum_{k=0}^{48} C(48,k) * C(48 + 2n - k - 1, 2n - k).
 * The helper function binomial(idx_t a, idx_t b) (in utils/binomial.h)
 * is used to evaluate binomial coefficients in arbitrary precision.
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    const idx_t LIM = 48;                /* exponent of (1+x) and (1-x) */
    idx_t twon = n << 1;                 /* 2*n, safe in idx_t          */
    seq_t result = 0;

    for (idx_t k = 0; k <= LIM && k <= twon; ++k) {
        seq_t c1 = binomial(LIM, k);                 /* (1+x)^48 coefficient   */
        idx_t m  = twon - k;                        /* remaining power from (1-x)^(-48) */
        /* coefficient of x^m in (1-x)^(-48) : binomial(48+m-1, m) */
        seq_t c2 = binomial(LIM + m - 1, m);
        result += c1 * c2;
    }
    return result;
}

/*
 * A035786  Coordination sequence for C_49 lattice.
 * https://oeis.org/A035786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^49
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* compute sum_{k=0}^{49} binom(49,k) * binom(48 + 2n - k, 2n - k) */
    const idx_t max_k = 49;
    idx_t two_n = n * 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        if (two_n < k) break;                 /* skip terms with negative exponent */
        idx_t denom = two_n - k;              /* 2n - k   */
        seq_t term = binomial((idx_t)49, k) * binomial(48 + denom, denom);
        sum += term;
    }
    return sum;
}

/*
 * A035749  Coordination sequence for C_12 lattice.
 * https://oeis.org/A035749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^12
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 64, u128 n <= 3668,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 9144 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t N = (idx_t)2 * n;               /* N = 2n */
    seq_t result = 0;

    /* binomial coefficients C(12,k), k = 0..12 */
    static const seq_t C12[13] = {
        (seq_t)1,  (seq_t)12,  (seq_t)66,  (seq_t)220,
        (seq_t)495, (seq_t)792, (seq_t)924, (seq_t)792,
        (seq_t)495, (seq_t)220, (seq_t)66,  (seq_t)12,
        (seq_t)1
    };

    /* k runs from 0 to min(12,N) */
    idx_t kmax = 12;
    if (N < 12)
        kmax = N;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* binom(11+N-k, N-k) returned as seq_t */
        seq_t term = C12[k] * binomial(11 + N - k, N - k);
        result += term;
    }

    return result;
}

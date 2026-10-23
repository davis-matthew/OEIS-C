/*
 * A126925  Coordination sequence for 24-dimensional cyclotomic lattice
 *          Z[zeta_35].
 * https://oeis.org/A126925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{min(n,24)} c_k * binom(n - k + 23, 23) where the
 *            coefficient array c_k (k=0..24) is [1, 11, 66, 286, 1001, 2996,
 *            7896, 18631, 39671, 76046, 128726, 185206, 212926, 185206,
 *            128726, 76046, 39671, 18631, 7896, 2996, 1001, 286, 66, 11, 1]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Use 64‑bit constants for the coefficients.  Coefficients larger than
       2^8 are fine because for a(n) ≤ 2^8 only the first three coefficients
       are needed.  For larger n the result does not fit in seq_t when
       seq_t is 8 bits, so truncation is acceptable. */
    static const uint64_t coef[25] = {
        1ULL, 11ULL, 66ULL, 286ULL, 1001ULL, 2996ULL, 7896ULL,
        18631ULL, 39671ULL, 76046ULL, 128726ULL, 185206ULL,
        212926ULL, 185206ULL, 128726ULL, 76046ULL, 39671ULL,
        18631ULL, 7896ULL, 2996ULL, 1001ULL, 286ULL,
        66ULL, 11ULL, 1ULL
    };

    seq_t result = 0;
    idx_t lim = (n < 24) ? n : 24;           /* k runs 0..lim */
    for (idx_t k = 0; k <= lim; ++k) {
        idx_t m = n - k + 23;                /* n-k ≥ 0 because k ≤ lim ≤ n */
        seq_t bin = binom(m, 23);            /* binomial coefficient */
        result += (seq_t)coef[k] * bin;
    }
    return result;
}

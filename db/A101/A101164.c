/*
 * A101164  Triangle read by rows: Delannoy numbers minus binomial
 *          coefficients.
 * https://oeis.org/A101164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 125, u32 n <= 417, u64 n <= 1455,
 *            u128 n <= 5150, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101164 (triangle read by rows).
   a(n) = T(r,k),   with r = floor((sqrt(8n+1)-1)/2),
   k = n - r*(r+1)/2.
   T(r,k) = sum_{j=0}^{r-k} binom(r-k,j)*binom(k,j)*2^j - binom(r,k). */

#include <math.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute row r and column k of the flattened triangle */
    long double sq = sqrt((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((sq - 1.0L) / 2.0L);          /* row index */
    idx_t start = r * (r + 1) / 2;                 /* first index of row r */
    idx_t k = n - start;                           /* column index in row r */

    /* Compute T(r,k) using the defining sum */
    seq_t sum = 0;
    idx_t maxj = r - k;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term = binom(r - k, j);
        term *= binom(k, j);
        term *= pow2(j);
        sum += term;
    }
    seq_t result = sum - binom(r, k);
    return result;
}

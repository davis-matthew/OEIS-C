/*
 * A101164  Triangle read by rows: Delannoy numbers minus binomial
 *          coefficients.
 * https://oeis.org/A101164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 125, u32 n <= 417, u64 n <= 1455,
 *            u128 n <= 5150, big any n
 * Validated: standard, pass, 94 terms through n = 4157 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101164 (triangle read by rows).
   a(n) = T(r,k),   with r = floor((sqrt(8n+1)-1)/2),
   k = n - r*(r+1)/2.
   T(r,k) = sum_{j=0}^{r-k} binom(r-k,j)*binom(k,j)*2^j - binom(r,k). */

#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"
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
        seq_t term = binomial(r - k, j);
        term *= binomial(k, j);
        term *= pow2(j);
        sum += term;
    }
    seq_t result = sum - binomial(r, k);
    return result;
}

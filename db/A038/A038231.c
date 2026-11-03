/*
 * A038231  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j).
 * https://oeis.org/A038231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the row‑wise enumeration of the triangle * T(i,j) =
 *            binomial(i,j) * 4^(i-j). * For a given n, let r =
 *            floor((sqrt(8*n+1)-1)/2) and * j = n - r*(r+1)/2, then a(n) =
 *            T(r,j)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 121, u64 n <= 438,
 *            u128 n <= 1660, big any n
 * Validated: standard, pass, 71 terms through n = 7313 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Determine the row r that contains the n-th term. */
    double d = sqrt((double)8 * (double)n + 1.0);          /* sqrt(8*n+1) */
    idx_t sqrtDelta = (idx_t)d;                            /* floor(sqrt(8*n+1)) */
    idx_t r = (sqrtDelta - 1) / 2;                         /* row index */

    /* Position j within the row r. */
    idx_t prev_rows = r * (r + 1) / 2;                     /* elements before row r */
    idx_t j = n - prev_rows;                               /* column index */

    /* Compute the binomial coefficient and the power of 4. */
    seq_t bin = binomial(r, j);
    seq_t pow4_val = pow4(r - j);

    /* Return the product. */
    return seq_mul(bin, pow4_val);
}

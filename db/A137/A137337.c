/*
 * A137337  T(i,j) = (-1)^(i+j)*(i+1)*binomial(i,j)*2^(i-j)*4^j.
 * https://oeis.org/A137337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = T(i,j) where i = floor((sqrt(8n+1)-1)/2), * j = n -
 *            i*(i+1)/2, * and T(i,j) =
 *            (-1)^(i+j)*(i+1)*binomial(i,j)*2^(i-j)*4^j
 * Range:     i8 n <= 6, i16 n <= 23, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, isqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    /* Find the row i that contains index n */
    idx_t i = (isqrt(8 * n + 1) - 1) / 2;

    /* The first index of row i */
    idx_t offset = i * (i + 1) / 2;

    /* Column index within that row */
    idx_t j = n - offset;

    /* Compute the triangle entry */
    seq_t val = (seq_t)(i + 1) * binomial(i, j);
    val *= pow2((idx_t)(i - j));
    val *= pow4(j);

    /* Apply the sign factor */
    if (((i + j) & 1) != 0)
        val = -val;

    return val;
}

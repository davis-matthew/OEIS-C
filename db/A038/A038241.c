/*
 * A038241  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*11^j.
 * https://oeis.org/A038241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*4^(i-j)*11^j, where i is the largest
 *            integer with i*(i+1)/2 ≤ n, and j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Find row i */
    idx_t i = 0;
    while (((__uint128_t)(i + 1) * (i + 2)) / 2 <= n) {
        ++i;
    }

    /* Column j */
    idx_t j = n - (idx_t)((( __uint128_t)i * (i + 1)) / 2);

    /* Compute entry: binomial(i,j) * 4^(i-j) * 11^j */
    seq_t term = binomial(i, j);
    term = seq_mul(term, ipow_u64_idx(4ULL, i - j));
    term = seq_mul(term, ipow_u64_idx(11ULL, j));

    return term;
}

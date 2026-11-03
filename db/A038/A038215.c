/*
 * A038215  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*9^j.
 * https://oeis.org/A038215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i,j) * 2^(i-j) * 9^j, where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2 Corresponds to
 *            row‑major enumeration of the triangle T(i,j)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-01)
 * Helpers:   binomial, ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Find row index i such that n is within row i */
    idx_t i = 0;
    while ((unsigned __int128)(i + 1) * (i + 2) / 2 <= n)
        i++;

    idx_t triangular = (idx_t)((unsigned __int128)i * (i + 1) / 2);
    idx_t j = n - triangular;

    seq_t val = binomial(i, j);
    val = seq_mul(val, pow2(i - j));
    val = seq_mul(val, ipow_idx((seq_t)9, j));
    return val;
}

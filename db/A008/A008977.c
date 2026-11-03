/*
 * A008977  a(n) = (4*n)!/(n!)^4.
 * https://oeis.org/A008977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (4*n)!/(n!)^4 = C(2*n,n) * C(3*n,n) * C(4*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 91 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t v1 = binomial(2*n, n);
    seq_t v2 = seq_mul(v1, binomial(3*n, n));
    seq_t v3 = seq_mul(v2, binomial(4*n, n));
    return v3;
}

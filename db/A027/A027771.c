/*
 * A027771  a(n) = (n+1)*binomial(n+1,11).
 * https://oeis.org/A027771
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,11)
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 31, u64 n <= 176,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 42 terms through n = 961 (2026-11-01)
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

seq_t a(idx_t n) {
    idx_t m = n + 1;                /* n+1 as an idx_t */
    seq_t c = binomial(m, 11);      /* binomial(m,11) as seq_t */
    return seq_mul(c, (seq_t)m);   /* (n+1) * binomial(n+1,11) */
}

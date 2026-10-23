/*
 * A101863  Main diagonal of A101858.
 * https://oeis.org/A101863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + floor(n*(1 + sqrt(5))/2)^2
 * Range:     u8 n <= 8, u16 n <= 134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the main diagonal of A101858 via Beatty sequence of
   the golden ratio.  Uses integer arithmetic only. */

#include <stdint.h>
#include "../utils/integer_sqrt.h"  /* integer_sqrt(idx_t x) */

/* Helper: compute floor(n * φ) where φ = (1 + √5)/2.
   Uses identity floor(n*φ) = (n + floor(n*√5)) / 2. */
static inline seq_t floor_n_phi(idx_t n)
{
    /* Compute floor(n * √5) as floor(√(5 * n^2)). */
    /* 5 * n^2 is small enough for idx_t in expected use cases
       (n up to ~10 000). */
    idx_t prod = 5 * n * n;
    idx_t sqrt5n = integer_sqrt(prod);          /* floor(n*√5) */
    seq_t term = ((seq_t)n + (seq_t)sqrt5n) / 2;/* floor(n*φ)   */
    return term;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2  */
    seq_t k  = floor_n_phi(n);              /* floor(n*φ) */
    seq_t kk = k * k;                       /* floor(n*φ)^2 */
    return nn + kk;                         /* a(n)  */
}

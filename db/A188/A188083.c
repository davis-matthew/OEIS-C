/*
 * A188083  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=2, [ ]=floor.
 * https://oeis.org/A188083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188083:
 * a(n) = floor((n+2)*sqrt(3)) - floor(n*sqrt(3)) - floor(2*sqrt(3))
 *   = floor(sqrt(3*(n+2)^2)) - floor(sqrt(3*n^2)) - 3
 *
 * Uses integer square‐root via the helper function sqrt_floor().
 * All calculations are performed in seq_t to avoid overflow. */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as required by sequence definition */
    /* Compute floor((n+2)*sqrt(3)) = floor(sqrt(3*(n+2)^2)) */
    idx_t n2 = n + 2;                      /* n+2 fits in idx_t */
    seq_t x1 = (seq_t)3 * (seq_t)n2 * (seq_t)n2;
    seq_t t1 = sqrt_floor(x1);

    /* Compute floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t x2 = (seq_t)3 * (seq_t)n * (seq_t)n;
    seq_t t2 = sqrt_floor(x2);

    /* floor(2*sqrt(3)) = 3, constant */
    return t1 - t2 - 3ULL;
}

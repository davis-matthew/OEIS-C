/*
 * A024540  a(n) = Sum_{k=1..n} floor( 1/{k*sqrt(2)} ), where {x} := x -
 *          floor(x).
 * https://oeis.org/A024540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(1 / frac(k*sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 397, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The summand can be expressed without floating point as: */
/*  floor(1/frac(k*sqrt2)) = floor( (2*floor(k*sqrt2)) / ( 2*k*k - floor(k*sqrt2)^2 ) ) */

#include <stdint.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* k^2 as seq_t */
        seq_t k_sq = (seq_t)k * (seq_t)k;
        /* floor(k*sqrt(2)) = floor(sqrt(2*k^2)) */
        seq_t m = sqrt_floor( ((seq_t)2) * k_sq );
        /* denominator d = 2*k*k - m*m */
        seq_t d = ((seq_t)2 * k_sq) - (m * m);
        /* compute summand = floor( (2*m) / d ) */
        seq_t term = ((seq_t)2 * m) / d;
        sum += term;
    }
    return sum;
}

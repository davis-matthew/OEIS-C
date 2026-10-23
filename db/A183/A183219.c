/*
 * A183219  Complement of the heptagonal (7-gonal) numbers.
 * https://oeis.org/A183219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(1/2 + sqrt(2n/5))
 * Range:     u8 n <= 31, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute s = floor( sqrt(2n/5) ) using integer arithmetic.
       We use the identity: sqrt(2n/5) = sqrt((8n)/5)/2.              */
    seq_t t = ((seq_t)n * 8) / 5;   /* floor( (8n)/5 )                           */
    seq_t sqrt_t = floor_sqrt(t);   /* floor( sqrt( (8n)/5 ) )                 */
    seq_t s = sqrt_t / 2;           /* floor( sqrt(2n/5) )                     */

    /* Determine whether to round up: round up iff
       8n >= 20s^2 + 20s + 5                               */
    seq_t eight_n = (seq_t)n * 8;
    seq_t rhs = (seq_t)20 * s * s;
    rhs += (seq_t)20 * s;
    rhs += 5;
    seq_t k = (eight_n >= rhs) ? (s + 1) : s;

    return (seq_t)n + k;
}

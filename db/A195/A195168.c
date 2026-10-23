/*
 * A195168  a(n) = 3*n - floor(n*r/3), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*(1+sqrt5)/3)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses integer arithmetic and the utility floor_sqrt() to
   evaluate floor(n*sqrt5). */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                     /* convert index to seq_t       */
    seq_t temp = (seq_t)5 * ns * ns;          /* 5 * n^2                       */
    seq_t sqrt5n = floor_sqrt(temp);          /* floor(n*sqrt5)                */
    seq_t sum = ns + sqrt5n;                  /* n + floor(n*sqrt5)            */
    seq_t k = sum / (seq_t)6;                 /* floor((n + floor(n*sqrt5))/6) */
    return (seq_t)3 * ns - k;                 /* final value                   */
}

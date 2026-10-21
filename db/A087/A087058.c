/*
 * A087058  Smallest square number greater than 2*n^2.
 * https://oeis.org/A087058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + floor[n*sqrt(2)])^2
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: smallest square greater than 2*n^2 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;     /* convert index to seq_t */
    seq_t X = t * t;        /* n^2 */
    X *= 2;                 /* 2*n^2 */

    seq_t s0 = floor_sqrt(X); /* largest integer with s0^2 <= X */
    seq_t s  = s0 + 1;         /* smallest integer with s^2 > X */

    return s * s;                /* s^2 is the desired term */
}

/*
 * A184108  n + floor(3*sqrt(n-1)); complement of A184109.
 * https://oeis.org/A184108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(3*sqrt(n-1))
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handles n >= 1; behaviour for n == 0 is undefined but we return 0. */
    if (n == 0) return (seq_t)0;
    seq_t m    = (seq_t)(n - 1);        /* n-1 in seq_t */
    seq_t prod = (seq_t)9 * m;          /* 9*(n-1) in seq_t */
    seq_t f    = floor_sqrt(prod);      /* floor(3*sqrt(n-1)) */
    return (seq_t)n + f;                /* n + floor(3*sqrt(n-1)) */
}

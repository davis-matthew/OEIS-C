/*
 * A184584  floor[(n-1/3)r], where r=sqrt(5); complement of A184585.
 * https://oeis.org/A184584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - 1/3) * sqrt(5))
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is undefined. */
    seq_t M = (seq_t)n * 3 - 1;          /* M = 3n - 1 */
    seq_t t = 5 * M * M;                 /* t = 5 * M^2  */
    seq_t s = floor_sqrt(t);             /* s = floor(sqrt(t)) = floor(M * sqrt(5)) */
    return s / 3;                        /* floor((M * sqrt(5)) / 3) */
}

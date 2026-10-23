/*
 * A023970  First bit in fractional part of binary expansion of cube root of
 *          n.
 * https://oeis.org/A023970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the first fractional binary digit of ∛n is 1,
 *            otherwise 0. Equivalently, a(n) = 1 if n ≥ ceil((⌊∛n⌋ + 1/2)^3),
 *            else 0
 * Range:     u8 n <= 4134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    /* integer cube root of n */
    seq_t m = (seq_t)int_cube_root(n);       /* m = floor(∛n) */
    seq_t m2 = m * m;                        /* m^2 */
    seq_t m3 = m2 * m;                       /* m^3 */

    /* compute ceil((m + 1/2)^3) = m^3 + (12*m^2 + 6*m + 8)/8  */
    seq_t part = (seq_t)12 * m2 + (seq_t)6 * m + (seq_t)8;
    seq_t threshold = m3 + part / (seq_t)8;   /* integer division truncates */

    return (n >= (idx_t)threshold) ? (seq_t)1 : (seq_t)0;
}

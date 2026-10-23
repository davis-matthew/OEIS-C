/*
 * A186741  Expansion of f(x^5, x^7) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A186741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ integer k such that 6*k^2 - k = n, 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 666, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t D = (seq_t)n * 24 + 1;          /* discriminant 1 + 24*n */
    seq_t s = floor_sqrt(D);              /* integer square root of D */
    if (s * s != D)                       /* not a perfect square */
        return 0;
    seq_t m = s % 12;                     /* residue modulo 12 */
    return (m == 1 || m == 11) ? 1 : 0;   /* characteristic function */
}

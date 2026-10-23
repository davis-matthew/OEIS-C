/*
 * A171970  Integer part of the height of an equilateral triangle with side
 *          length n.
 * https://oeis.org/A171970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(3)/2)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

/* Computes the integer part of the height of an equilateral triangle with side length n.
   a(n) = ⌊n·√3/2⌋.  The computation is performed with integer arithmetic only. */
seq_t a(idx_t n)
{
    /* Use the identity: a(n) = ⌊⌊n·√12⌋ / 4⌋.
       Since   n·√12 = √(12·n²), we first compute 12·n² (fits in uint64_t for n≤10⁴).   */
    idx_t val = 12 * n * n;            /* val = 12·n²  (uint64_t)                 */
    idx_t k   = _floor_isqrt_u64(val); /* k  = ⌊√val⌋ = ⌊n·√12⌋                       */
    return (seq_t)(k / 4);             /* a(n) = ⌊k/4⌋ = ⌊n·√3/2⌋                     */
}

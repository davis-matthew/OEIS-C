/*
 * A054406  Beatty sequence for (3+sqrt 3)/2; complement of A022838.
 * https://oeis.org/A054406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 4836, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A054406 – Beatty sequence for (3 + √3)/2.
 *
 * a(n) = ⌊ n · (3 + √3)/2 ⌋
 *      = ⌊ (3n + ⌊ n·√3 ⌋) / 2 ⌋
 *
 * The function uses only integer arithmetic.  It relies on the
 * helper routine `sqrt_floor()` which returns ⌊√x⌋ for an arbitrary
 * sequence type `seq_t`.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;            /* cast index to seq_t          */
    seq_t n_sq = ns * ns;           /* n²                            */
    seq_t s = sqrt_floor(3 * n_sq); /* floor(n · √3) = ⌊√(3n²)⌋    */
    seq_t k = 3 * ns + s;           /* 3n + ⌊n√3⌋                   */
    if ((k & 1) == 0)               /* k even: exact division by 2   */
        return k >> 1;
    else                            /* k odd: subtract 1, divide     */
        return (k - 1) >> 1;
}

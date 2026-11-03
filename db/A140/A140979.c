/*
 * A140979  a(n) = floor(2*phi*floor(n*phi)) where phi = A001622.
 * https://oeis.org/A140979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*phi*floor(n*phi)) phi = (1+sqrt(5))/2
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

static seq_t _floor_sqrt5_mult(seq_t k)
{
    seq_t t = (k * k) * (seq_t)5;   /* 5*k*k */
    return isqrt_seq(t);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* optional, offset is 1 */

    seq_t nn  = (seq_t)n;
    seq_t fn  = _floor_sqrt5_mult(nn);          /* floor(n*sqrt5)   */
    seq_t m   = (nn + fn) / 2;                  /* floor(n*phi)     */
    seq_t fm  = _floor_sqrt5_mult(m);           /* floor(m*sqrt5)   */
    return m + fm;                              /* a(n)             */
}

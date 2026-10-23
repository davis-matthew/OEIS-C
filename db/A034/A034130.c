/*
 * A034130  Decimal part of cube root of a(n) starts with 4: first term of
 *          runs.
 * https://oeis.org/A034130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((10*n+4)^3 + 999) / 1000 (for n >= 1)
 * Range:     u8 none, u16 n <= 3, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t t = n_seq * 10 + 4;          /* t = 10*n + 4 */
    seq_t t_sq = t * t;                /* t^2 */
    seq_t t_cu = t_sq * t;             /* t^3 */
    return (t_cu + 999) / 1000;        /* ceil((t^3)/1000) – equivalent to the desired lower bound */
}

/*
 * A207672  n + [ns/r] + [nt/r], where []=floor, r=5, s=(1+sqrt(5))/2, t=1/s.
 * https://oeis.org/A207672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*s/5) + floor(n*t/5) where s = (1+sqrt(5))/2,
 *            t = 1/s. This reduces to a(n) = n + floor((n +
 *            floor(n*sqrt5))/10) + floor((floor(n*sqrt5) - n)/10)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n as seq_t to allow big‑integer operations */
    seq_t nn = (seq_t)n;

    /* Compute F = floor(n * sqrt(5)) via integer sqrt of 5*n^2 */
    seq_t val = nn * nn;  /* n^2 */
    val = val * 5;        /* 5*n^2 */
    seq_t F   = floor_sqrt(val);   /* floor(n*sqrt(5)) */

    /* floor((n + F)/10) and floor((F - n)/10) are integer divisions */
    seq_t part1 = (nn + F) / 10;
    seq_t part2 = (F - nn) / 10;

    return nn + part1 + part2;
}

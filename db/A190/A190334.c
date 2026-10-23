/*
 * A190334  a(n) = n + floor(n*r/t) + floor(n*s/t) where r=1, s=sqrt(3),
 *          t=1/s.
 * https://oeis.org/A190334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + floor(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Computes the OEIS sequence A190334:
 *   a(n) = 4*n + floor(n*sqrt(3))
 * for any n >= 1 (index starts at 1).  The computation uses
 * integer arithmetic only; the floor of n*sqrt(3) is obtained as
 * floor(sqrt(3*n^2)) via the provided floor_sqrt routine.  All
 * intermediate values are cast to seq_t to respect the build‑time
 * integer width or big‑int class. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t prod = 3 * nn * nn;     /* 3*n^2  (may overflow if seq_t is 128‑bit, but within bounds for typical use) */
    seq_t flr = floor_sqrt(prod); /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    return 4 * nn + flr;          /* 4*n + floor(n*sqrt(3)) */
}

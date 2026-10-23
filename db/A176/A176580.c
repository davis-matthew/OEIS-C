/*
 * A176580  n^3+Largest square, (Largest square <= n^3).
 * https://oeis.org/A176580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^3 + floor(sqrt((n-1)^3))^2
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176580: n^3 + Largest square <= n^3 (with offset 1). */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* offset: a(1)=0 corresponds to (1-1)^3 */
    idx_t m = n - 1;
    seq_t mm = (seq_t)m;
    seq_t m3 = mm * mm * mm;            /* (n-1)^3 */
    seq_t k  = floor_sqrt(m3);          /* floor(sqrt((n-1)^3)) */
    seq_t sq = k * k;                   /* square of that integer */
    return m3 + sq;                     /* final value */
}

/*
 * A186513  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=4+5j^2. Complement of
 *          A186514.
 * https://oeis.org/A186513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((n^2)/5-9/10))
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* For n = 1, 2 the inner square‑root argument is negative, so the
       floor of the square‑root is defined to be 0.  The sequence
       values are just n in those cases. */
    if (n < 3) return (seq_t)n;

    seq_t nn       = n;
    seq_t numerator = 2 * nn * nn - 9;      /* 2 n^2 - 9   */
    seq_t q          = numerator / 10;      /* floor((2 n^2-9)/10) */
    seq_t s          = sqrt_floor(q);       /* floor(sqrt(q)) */
    return nn + s;
}

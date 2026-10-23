/*
 * A186540  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-2+3j^2. Complement
 *          of A186539.
 * https://oeis.org/A186540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(3*n*n - 3/2))
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset is 1; for n<1 the result is undefined, return 0 */
    if (n == 0ULL)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;   /* n*n */
    t = t * 3;           /* 3*n*n */
    t -= 1;              /* 3*n*n - 1  (same floor as 3*n*n - 1.5) */
    seq_t s = floor_sqrt(t); /* floor(sqrt(3*n*n - 1)) */
    return nn + s;
}

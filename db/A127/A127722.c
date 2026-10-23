/*
 * A127722  Floor of square root of sum of squares of n consecutive odd
 *          numbers.
 * https://oeis.org/A127722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( n*(4n^2-1)/3 ) )
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, but handles n=0 safely */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;                      /* n^2 */
    seq_t t = nn2 * 4;                        /* 4n^2 */
    t -= 1;                                   /* 4n^2 - 1 */
    seq_t val;
    if ((n % 3) == 0)                         /* n divisible by 3 → divide n first */
        val = (nn / 3) * t;                   /* (n/3) * (4n^2-1) */
    else
        val = nn * (t / 3);                   /* n * ((4n^2-1)/3) */
    return floor_sqrt(val);
}

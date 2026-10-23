/*
 * A186389  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=6i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186390.
 * https://oeis.org/A186389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt((8*6*n) + 1) - 1) / 2)
 * Range:     u8 n <= 1, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = n + floor((sqrt(48*n + 1) - 1) / 2) */
/* where n is the index of the 6*i sequence and the sqrt is taken
   over the integer domain using floor_sqrt(seq_t). */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * 6;            /* value 6*n */
    seq_t y = 8 * v + 1;              /* 8*6*n + 1 = 48*n + 1 */
    seq_t s = floor_sqrt(y);          /* floor(sqrt(y))  */
    seq_t t = (s - 1) / 2;            /* number of triangular <= v */
    return t + (seq_t)n;              /* rank of 6*n in the joint sequence */
}

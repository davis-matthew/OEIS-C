/*
 * A186512  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-4+5j^2. Complement
 *          of A186511.
 * https://oeis.org/A186512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(5*n^2 - 5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute 5*n^2 - 5, using seq_t arithmetic to avoid overflow */
    seq_t val = sn * sn * (seq_t)5 - (seq_t)5;
    /* floor_sqrt is provided by the utilities */
    seq_t sqrtv = floor_sqrt(val);
    return sn + sqrtv;
}

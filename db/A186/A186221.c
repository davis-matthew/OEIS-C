/*
 * A186221  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          squares. Complement of A186222.
 * https://oeis.org/A186221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt((n^2+n)/2))
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t k = (m * (m + 1)) / 2;
    seq_t s = floor_sqrt(k);
    return m + s;
}

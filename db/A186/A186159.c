/*
 * A186159  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          octagonal numbers. Complement of A186274.
 * https://oeis.org/A186159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + the number of octagonal numbers < T_n, where T_n =
 *            n(n+1)/2. In other words, a(n) is the rank of the n-th
 *            triangular number in the ordered merge of triangular and
 *            octagonal numbers, with triangular numbers coming first in case
 *            of ties
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   count_oct_less_than, oct_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_oct_less_than.h"
#include "../utils/oct_value.h"

seq_t a(idx_t n)
{
    /* Triangular number T_n = n(n+1)/2 */
    unsigned __int128 T = (unsigned __int128)n * ((unsigned __int128)n + 1) / 2;

    /* Count octagonal numbers strictly less than T */
    idx_t oct_cnt = count_oct_less_than(T);

    /* Ranking: n (triangulars up to T) + oct_cnt (strictly smaller octagonals) */
    idx_t result = n + oct_cnt;
    return (seq_t)result;
}

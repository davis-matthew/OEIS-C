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
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 oct_value(idx_t m)
{
    /* m(3m-2) – octagonal number */
    return (unsigned __int128)m * ((unsigned __int128)3 * m - 2);
}

/* Count of octagonal numbers strictly less than a given value X.
   X is assumed to fit in seq_t metadata (but we do calculations in __int128). */
static inline idx_t count_oct_less_than(unsigned __int128 X)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Find an upper bound for the search by exponential growth */
    while (oct_value(high) < X) {
        /* avoid overflow of high for very large values */
        if (high > ((idx_t)-1) / 2) {
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }
    /* Binary search for the largest m with oct_value(m) < X */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (oct_value(mid) < X)
            low = mid;
        else
            high = mid;
    }
    return low; /* number of octagonal numbers < X */
}

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

/*
 * A186514  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=4+5j^2. Complement of
 *          A186513.
 * https://oeis.org/A186514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor_sqrt(5n²+4n – 1) + floor_sqrt((5n²+4n – 5)/5) +
 *            (is_square?2:1)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* V = 4 + 5*n^2  (value of 4 + 5*n^2) */
    seq_t V = (seq_t)5 * nn * nn + (seq_t)4;

    /* Count of i with i² < V  */
    seq_t s1 = floor_sqrt(V - (seq_t)1);

    /* Count of j with 4 + 5*j² < V  (integer division truncates) */
    seq_t s2 = floor_sqrt((V - (seq_t)5) / (seq_t)5);

    /* Check if V itself is a square (needed for tie handling) */
    seq_t r = floor_sqrt(V);
    seq_t is_square = (r * r == V) ? (seq_t)1 : (seq_t)0;

    /* Combine contributions */
    return s1 + s2 + (is_square ? (seq_t)2 : (seq_t)1);
}

/*
 * A168311  Numbers that are not sums of two triangular numbers and are not
 *          one greater than a sum of two triangular numbers.
 * https://oeis.org/A168311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m≥1 such that m and m-1 are not
 *            expressible as a sum of two triangular numbers T(i)+T(j), i,j≥0
 * Range:     u8 none, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches sequentially from 1 until it has found the
   n‑th such integer.  The implementation is O(n · √m), fast enough
   for n≤10000. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Return true iff x is a triangular number: k(k+1)/2 = x for some k≥0 */
static bool is_triangular(seq_t x)
{
    seq_t D = x * (seq_t)8 + (seq_t)1;     /* discriminant of k^2 + k - 2x = 0 */
    seq_t s = floor_sqrt(D);
    if (s * s != D) return false;          /* not a perfect square */
    return (((seq_t)s - (seq_t)1) & (seq_t)1) == 0;
}

/* Return true iff x can be written as T(i)+T(j) for some i,j≥0 */
static bool is_sum_two_triangular(seq_t x)
{
    /* iterate i and test T(i) + T(j) = x */
    for (idx_t i = 0; ; ++i)
    {
        /* T(i) = i*(i+1)/2 -- compute using seq_t for safety */
        seq_t ti = ((seq_t)i * (seq_t)(i + 1)) / (seq_t)2;
        if (ti > x) break;
        seq_t rest = x - ti;
        if (is_triangular(rest)) return true;
    }
    return false;
}

/* Return the nth term (1‑based).  For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    idx_t m = 0;

    while (count < n)
    {
        ++m;
        if (!is_sum_two_triangular((seq_t)m) &&
            !is_sum_two_triangular((seq_t)(m - 1))) {
            ++count;
        }
    }
    return (seq_t)m;
}

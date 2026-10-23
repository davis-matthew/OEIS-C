/*
 * A116666  Triangle, row sums = number of edges in n-dimensional hypercubes.
 * https://oeis.org/A116666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r-1, c-1)*(2*c-1) where r = smallest integer with
 *            r(r+1)/2 >= n c = n - r(r-1)/2 (1-based column index)
 * Range:     u8 n <= 32, u16 n <= 127, u32 n <= 480, u64 n <= 1981,
 *            u128 n <= 7875, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by scanning rows in O(√n) time. */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative number of entries up to that row
       is >= n.  Use a simple O(√n) loop; n up to 10k makes this trivial. */
    idx_t r = 1;
    idx_t cum = 1;          /* sum of 1..r = r(r+1)/2 for r=1 */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Compute the column index c (1‑based) within row r */
    idx_t prev_cum = cum - r;   /* sum of entries in rows 1..r-1 */
    idx_t c = n - prev_cum;     /* 1 ≤ c ≤ r */

    /* Triangle value: binom(r-1, c-1) * (2*c-1) */
    seq_t val = binom(r - 1, c - 1);
    val *= (seq_t)(2 * c - 1);
    return val;
}

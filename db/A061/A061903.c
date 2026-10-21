/*
 * A061903  Number of distinct elements of the iterative cycle: n -> sum of
 *          digits of n^2.
 * https://oeis.org/A061903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_of_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iteratively compute the number of distinct values in the orbit of
   f(x) = sum of digits of (x²), starting from the first application
   of f to the index n.  The sequence is defined by
   a(n) = |{ fⁱ(n) for i ≥ 1 until the first repeated value }|. */

#include <stdint.h>
#include "../utils/digit_sum_of_square.h"

seq_t a(idx_t n)
{
    /* First step after the input value */
    seq_t cur = digit_sum_of_square((seq_t)n);

    /* All cycle elements are at most 351 for 64‑bit input values.
       For the purposes of this function (n ≤ 10000) the bound 400 is
       more than sufficient. */
    enum { MAXV = 400 };

    int pos[MAXV];
    for (int i = 0; i < MAXV; ++i)
        pos[i] = -1;

    int step = 0;
    for (;;)
    {
        int v = (int)cur;          /* cur is small (≤ 81 for n ≤ 10000) */
        if (pos[v] != -1)          /* repeated value – cycle reached */
            return (seq_t)step;     /* number of distinct values seen */

        pos[v] = step;             /* record first appearance */
        ++step;                    /* one more distinct value */
        cur = digit_sum_of_square(cur);
    }
}

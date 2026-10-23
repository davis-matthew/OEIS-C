/*
 * A157639  Least number of points in a square n X n lattice from which all
 *          other points of the square are visible.
 * https://oeis.org/A157639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nice, nonn
 * Formula:   * a(n) = 1 for n ≤ 3 * a(n) = smallest k ≥ 2 such that (k+1)! ≥
 *            n+1 * (i.e. k = min{ m-1 : m! ≥ n+1 })
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for the first three terms */
    if (n <= 3U)
        return (seq_t)1;

    /* Compute the target value n+1 in the wider type seq_t. */
    seq_t target = (seq_t)n + (seq_t)1;

    /* Start with m = 2 ⇒ 2! = 2 */
    idx_t m = 2U;
    seq_t fac = (seq_t)2;   /* current factorial value = m! */

    /* Increment m until m! ≥ target.  The loop is tiny (m ≤ 21 for all
     * practical n within 64‑bit limits). */
    while (fac < target) {
        ++m;
        fac = fac * (seq_t)m;  /* Update factorial sequentially */
    }

    /* The required number of points is k = m - 1. */
    return (seq_t)(m - 1U);
}

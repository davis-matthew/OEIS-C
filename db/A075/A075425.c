/*
 * A075425  Number of steps to reach 1 starting with n and iterating the map n
 *          ->rad(n)-1, where rad(n) is the squarefree kernel of n (A007947).
 * https://oeis.org/A075425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the steps to reach 1 by iterating n -> rad(n) - 1,
 *            where rad is the square‑free kernel
 * Range:     u8 n <= 1757, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t steps = 0;
    idx_t cur = n;

    /* The mapping strictly decreases; the loop terminates at 1. */
    while (cur > 1) {
        seq_t r = rad(cur);          /* r ∈ seq_t, r ≥ 1 and r ≤ cur   */
        cur = (idx_t)(r - 1);        /* next value is ≤ cur - 1           */
        ++steps;
    }
    return steps;
}

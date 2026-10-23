/*
 * A117831  Let S_n be the infinite sequence formed by starting with n and
 *          repeatedly reversing the digits and adding 4 to get the next term.
 *          Sequence gives number of steps for S_n to reach a cycle, or -1 if
 *          no cycle is ever reached.
 * https://oeis.org/A117831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = μ(n) where μ(n) is the number of steps required for the
 *            sequence defined by S_0 = n S_{k+1} = rev(S_k) + 4 to reach its
 *            first cycle. (If n already lies on a cycle, μ(n) = 0.) The
 *            calculation is performed using Floyd's cycle-finding algorithm
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rev.h"          /* rev(idx_t) */

seq_t a(idx_t n)
{
    /* Starting value 0 is a dummy and returns 0 steps. */
    if (n == 0) return (seq_t)0;

    /* Step 1: Find first meeting point of tortoise and hare. */
    idx_t tortoise = rev(n) + 4;
    idx_t hare     = rev(tortoise) + 4;   /* hare = f(f(n)) */

    while (tortoise != hare) {
        tortoise = rev(tortoise) + 4;
        idx_t temp = rev(hare) + 4;
        hare = rev(temp) + 4;
    }

    /* Step 2: Find start of cycle (μ). */
    idx_t mu = 0;
    tortoise = n;
    while (tortoise != hare) {
        tortoise = rev(tortoise) + 4;
        hare     = rev(hare) + 4;
        mu++;
    }

    /* Return the tail length as the result. */
    return (seq_t)mu;
}

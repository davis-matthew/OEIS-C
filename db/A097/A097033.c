/*
 * A097033  Number of transient terms before either 0 or a finite cycle is
 *          reached when unitary-proper-divisor-sum-function f(x) = A034460(x)
 *          is iterated and the initial value is n.
 * https://oeis.org/A097033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of transient terms before hitting 0 or a cycle
 *            when iterating f(x) = A034460(x) (sum of proper unitary
 *            divisors). Computed by iterative evaluation with cycle detection
 * Range:     u8 n <= 288, u16 n <= 9641, u32 n <= 9641, u64 n <= 9641,
 *            u128 n <= 9641, big any n
 * Helpers:   unitary_sum, unitary_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/unitary_sum.h"
#include "../utils/unitary_sum_uint64.h"

/* Threshold for visited array – far larger than any expected chain length
   for the test ranges. */
#define MAX_VISITED 1048576

/* Main sequence function.  It iterates f(x) starting from n,
   stores visited values in a table for linear search, and returns
   the number of distinct transient values before a repeat or 0. */
seq_t a(idx_t n)
{
    /* Sequence values are 1‑based. */
    seq_t cur = n;
    static seq_t visited[MAX_VISITED];
    int visited_size = 0;

    for (;;)
    {
        /* Terminate on 0. */
        if (cur == 0)
            return (seq_t) visited_size;

        /* Detect repetition. */
        for (int i = 0; i < visited_size; ++i)
        {
            if (visited[i] == cur)
                return (seq_t) i;    /* transient length = first index of cur */
        }

        /* Record current value. */
        if (visited_size >= MAX_VISITED)
            return (seq_t) visited_size;  /* safety fallback */

        visited[visited_size++] = cur;
        cur = unitary_sum(cur);
    }
}

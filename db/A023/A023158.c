/*
 * A023158  Number of cycles of function f(x) = x^7 mod n.
 * https://oeis.org/A023158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of cycles of the function f(x) = x^7 (mod n)
 * Range:     u8 n <= 351, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the distinct cycles in the functional graph
   defined on the set {0, 1, …, n‑1}.  For n ≥ 1 it is the sequence
   A023158 from the OEIS.  The implementation follows the standard
   DFS-with-state approach to detect new cycles in O(n) time. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 as a placeholder. */
    if (n == (idx_t)0) return (seq_t)0;

    size_t N = (size_t)n;
    /* State array: 0 = unvisited, 1 = in current stack, 2 = finished */
    uint8_t *state = (uint8_t *)malloc(N);
    if (!state) return (seq_t)0;            /* unlikely for n ≤ 10 000 */

    /* Temporary stack to remember the current walk before marking it finished */
    size_t *stack = (size_t *)malloc(N * sizeof(size_t));
    if (!stack) { free(state); return (seq_t)0; }

    memset(state, 0, N);

    seq_t cycles = 0;
    const seq_t mod = (seq_t)n;

    for (size_t i = 0; i < N; ++i) {
        if (state[i] != 0) continue;               /* already processed */

        size_t top = 0;
        size_t cur = i;

        /* Follow the function until we hit a previously seen node. */
        while (state[cur] == 0) {
            state[cur] = 1;                        /* mark as visiting */
            stack[top++] = cur;                    /* remember for later */

            /* compute f(cur) = cur^7 (mod n) */
            seq_t x = (seq_t)cur;
            seq_t r = 1;
            for (int e = 0; e < 7; ++e) r = (r * x) % mod;
            cur = (size_t)r;
        }

        /* If we returned to a node that is still in the current path,
           we have discovered a new cycle. */
        if (state[cur] == 1) cycles++;

        /* Mark all nodes encountered in this walk as finished. */
        for (size_t j = 0; j < top; ++j) {
            state[stack[j]] = 2;
        }
    }

    free(state);
    free(stack);
    return cycles;
}

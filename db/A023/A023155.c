/*
 * A023155  Number of cycles of function f(x) = x^4 mod n.
 * https://oeis.org/A023155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of the map x→x^4 (mod n)
 * Range:     u8 n <= 603, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_pow4_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/next_pow4_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for n<1, though offset is 1 */

    /* state: 0 = unvisited, 1 = in current path, 2 = processed (cycle or tail) */
    unsigned char *state = (unsigned char*)malloc((size_t)n);
    if (!state) return 0;
    memset(state, 0, (size_t)n);

    seq_t cycles = 0;
    seq_t mod = (seq_t)n;

    for (idx_t i = 0; i < n; ++i)
    {
        if (state[i]) continue;          /* already processed */

        idx_t cur = i;

        /* Follow edges until we hit a processed node or revisit a node in this path */
        while (state[cur] == 0)
        {
            state[cur] = 1;              /* mark as visiting */
            cur = (idx_t)next_pow4_mod((seq_t)cur, mod);
        }

        /* If we returned to a node in the current path, we have discovered a new cycle */
        if (state[cur] == 1)
        {
            ++cycles;
            /* Mark all nodes in the cycle as processed (state=2) */
            idx_t cycle_node = cur;
            do
            {
                state[cycle_node] = 2;
                cycle_node = (idx_t)next_pow4_mod((seq_t)cycle_node, mod);
            } while (cycle_node != cur);
        }

        /* Mark the remaining nodes in the current path (tails) as processed */
        idx_t path_node = i;
        while (state[path_node] == 1)
        {
            state[path_node] = 2;
            path_node = (idx_t)next_pow4_mod((seq_t)path_node, mod);
        }
    }

    free(state);
    return cycles;
}

/*
 * A023154  Number of cycles of function f(x) = x^3 mod n.
 * https://oeis.org/A023154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 934, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of directed cycles in the functional graph of f(x)=x^3 mod n */
/* The graph has vertices 0..n-1. This implementation traverses each
   connected component once, counting the unique cycle in each. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_cube.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, return 0 for safety */

    /* Allocate visitation flags: 0 = unvisited, 1 = in current path,
       2 = finished.                                 */
    uint8_t *color = (uint8_t *)malloc((size_t)n);
    if (!color) return 0;
    /* Stack to record the current walk. */
    idx_t *stack = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!stack) { free(color); return 0; }

    for (size_t i = 0; i < (size_t)n; ++i) color[i] = 0;

    seq_t cycles = 0;

    for (idx_t start = 0; start < n; ++start)
    {
        if (color[(size_t)start]) continue;    /* already processed */

        size_t top = 0;
        idx_t cur = start;

        /* Walk until we hit a visited vertex. */
        while (color[(size_t)cur] == 0)
        {
            color[(size_t)cur] = 1;           /* mark as in current path */
            stack[top++] = cur;               /* push onto stack */
            cur = mod_cube(cur, n);           /* next value in the walk */
        }

        /* If we re‑encountered a vertex that is still in the current
           path, a new directed cycle was found. */
        if (color[(size_t)cur] == 1) cycles += 1;

        /* Mark all vertices in this walk as finished. */
        for (size_t i = 0; i < top; ++i)
            color[(size_t)stack[i]] = 2;
    }

    free(color);
    free(stack);
    return cycles;
}

/*
 * A220844  Sum of inclusive heights of complete 4-ary trees on n nodes.
 * https://oeis.org/A220844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of inclusive heights of all nodes in the complete
 *            4‑ary tree containing n nodes (root at depth 0, height = #nodes
 *            on longest path to a leaf inclusive)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* not used but safe */

    /* Allocate an array of heights; we can safely use malloc because
       we assign to every element before reading it. */
    seq_t *h = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!h)
        return 0;                          /* allocation failure */

    seq_t sum = 0;

    /* Process nodes from the last to the root so that children's
       heights are known when computing a parent's height.          */
    for (idx_t i = n; i-- > 0; ) {
        idx_t first_child = 4 * i + 1;
        seq_t max_child = 0;

        if (first_child < n) {
            /* For k‑ary tree, children are consecutive.            */
            for (idx_t j = 0; j < 4; ++j) {
                idx_t child = first_child + j;
                if (child >= n)
                    break;
                if (h[child] > max_child)
                    max_child = h[child];
            }
        }
        h[i] = max_child + 1;     /* height of this node          */
        sum += h[i];
    }

    free(h);
    return sum;
}

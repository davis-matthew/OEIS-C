/*
 * A065631  Permutation of N induced by rotating the node 4 right in the
 *          infinite planar binary tree. The fourth row of A065625. Inverse of
 *          A065632.
 * https://oeis.org/A065631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the new label of node n after a right rotation of node 4
 *            in the infinite planar binary tree. The mapping is obtained by a
 *            breadth‑first traversal of the rotated tree
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   left_child, right_child
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/left_child.h"
#include "../utils/right_child.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;   /* root stays at position 1 */

    /* Dynamic queue for breadth‑first traversal */
    size_t cap = 128;
    idx_t *queue = (idx_t*)malloc(cap * sizeof(idx_t));
    size_t head = 0, tail = 0;
    queue[tail++] = 1;             /* start from the root */

    seq_t label = 1;               /* label assigned to the current node */

    while (head < tail) {
        idx_t cur = queue[head++];

        if (cur == n) {            /* found the target node */
            free(queue);
            return label;
        }

        /* Enqueue children in left‑then‑right order */
        idx_t lc = left_child(cur);
        idx_t rc = right_child(cur);

        if (tail == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(queue, cap * sizeof(idx_t));
            if (!tmp) { free(queue); return (seq_t)0; }
            queue = tmp;
        }
        queue[tail++] = lc;

        if (tail == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(queue, cap * sizeof(idx_t));
            if (!tmp) { free(queue); return (seq_t)0; }
            queue = tmp;
        }
        queue[tail++] = rc;

        label++;                   /* next node in BFS order */
    }

    free(queue);                    /* should never reach here for n >= 1 */
    return (seq_t)0;
}

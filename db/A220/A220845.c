/*
 * A220845  Sum of exclusive heights of complete 3-ary trees on n nodes.
 * https://oeis.org/A220845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the subtree heights of all nodes in the complete
 *            3‑ary tree on n nodes. The height of a leaf node is 0; the
 *            height of an internal node is 1 plus the maximum height of its
 *            children. The value a(n) is the sum of these heights for all n
 *            nodes
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
    if (n == 0) return 0;       /* safety for n < 1, although the sequence starts at n = 1 */

    /* Allocate an array to hold the height of every node.
       For n up to about 10 000 the heights are very small, but we store them in seq_t
       to keep the code simple and type‑consistent. */
    seq_t *height = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!height) return 0;       /* allocation failure: return 0 as a fallback */

    seq_t sum = 0;

    /* Bottom‑up traversal: compute heights starting from the last node. */
    idx_t i = n;
    while (i-- > 0) {
        idx_t child1 = 3 * i + 1;
        idx_t child2 = child1 + 1;
        idx_t child3 = child1 + 2;

        seq_t max_child_height = 0;
        bool has_child = false;

        if (child1 < n) {
            has_child = true;
            if (height[child1] > max_child_height)
                max_child_height = height[child1];
        }
        if (child2 < n) {
            has_child = true;
            if (height[child2] > max_child_height)
                max_child_height = height[child2];
        }
        if (child3 < n) {
            has_child = true;
            if (height[child3] > max_child_height)
                max_child_height = height[child3];
        }

        if (has_child)
            height[i] = max_child_height + 1;
        else
            height[i] = 0;

        sum += height[i];
    }

    free(height);
    return sum;
}

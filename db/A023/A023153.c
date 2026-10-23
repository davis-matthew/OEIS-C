/*
 * A023153  Number of cycles of function f(x) = x^2 mod n.
 * https://oeis.org/A023153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of the mapping f(x) = x² mod n, where
 *            the graph is taken on the set {0,1,…,n‑1}. The sequence is
 *            indexed with offset 1, so a(1) corresponds to n = 1
 * Range:     u8 n <= 17, u16 n <= 665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, calloc, free */

seq_t a(idx_t n)
{
    /* Invalid or trivial input. For n = 0 the OEIS offset does not
       apply; we return 0 for completeness. */
    if (n == 0) return 0;

    /* Allocation: state array holds 0=unvisited, 1=visiting,
       2=finished.  It is zero‑filled with calloc. */
    unsigned char *state = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!state) return 0;                 /* allocation failure */

    /* Temporary buffer to remember the current traversal path. */
    idx_t *path = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!path) { free(state); return 0; }

    seq_t cycles = 0;                    /* result accumulator */

    for (idx_t start = 0; start < n; ++start) {
        if (state[start]) continue; /* already processed */

        size_t pos = 0;
        idx_t cur  = start;

        /* Follow the functional digraph until a visited node is reached. */
        while (state[cur] == 0) {
            state[cur] = 1;                 /* mark as visiting */
            path[pos++] = cur;              /* store for later cleanup */

            /* Compute next = cur² mod n. */
            seq_t prod = (seq_t)cur * (seq_t)cur;
            cur = (idx_t)(prod % (seq_t)n);
        }

        /* If we ended at a node still marked as visiting,
           a new cycle was found. */
        if (state[cur] == 1) cycles++;

        /* Mark all nodes in the current path as finished. */
        for (size_t j = 0; j < pos; ++j)
            state[path[j]] = 2;
    }

    free(path);
    free(state);
    return cycles;
}

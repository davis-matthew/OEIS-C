/*
 * A023159  Number of cycles of function f(x) = x^8 mod n.
 * https://oeis.org/A023159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of the function f(x) = x^8 mod n
 * Range:     u8 n <= 539, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct cycles in the functional graph
 * defined by f(x) = x^8 (mod n) on the set {0,1,...,n-1}.
 * The algorithm follows each unvisited vertex until it
 * either encounters an already processed vertex or a vertex
 * that is part of the current path (which indicates a new cycle).
 * Complexity O(n) for each call. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined, but safe */
    }

    /* state: 0 = unvisited, 1 = in current path, 2 = processed */
    uint8_t *state = (uint8_t *)malloc((size_t)n);
    if (!state) {
        return 0; /* allocation failure; return zero as fallback */
    }
    for (idx_t i = 0; i < n; ++i) {
        state[i] = 0;
    }

    /* temporary stack for the current traversal */
    idx_t *path = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!path) {
        free(state);
        return 0; /* allocation failure */
    }

    seq_t cycles = 0;

    for (idx_t start = 0; start < n; ++start) {
        if (state[start] != 0) {
            continue; /* already processed */
        }

        idx_t cur = start;
        idx_t path_len = 0;

        /* walk along the function until a visited vertex is found */
        while (state[cur] == 0) {
            state[cur] = 1;
            path[path_len++] = cur;

            /* next = cur^8 mod n */
            seq_t next = mod_pow((seq_t)cur, 8, (seq_t)n);
            cur = (idx_t)next; /* next is guaranteed < n */
        }

        /* if we re-enter a vertex in the current path -> new cycle */
        if (state[cur] == 1) {
            ++cycles;
        }

        /* mark all vertices in the current path as processed */
        for (idx_t i = 0; i < path_len; ++i) {
            state[path[i]] = 2;
        }
    }

    free(state);
    free(path);

    return cycles;
}

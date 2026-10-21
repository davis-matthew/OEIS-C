/*
 * A095031  An example of a (v,k,lambda)=(15,7,3) cyclic difference set.
 * https://oeis.org/A095031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the (15,7,3) cyclic difference set by exhaustive search.
   Returns the n‑th element (1‑based). Out of range gives 0. */

#include <stdbool.h>

#define V      15  /* group order  v */
#define K      7   /* block size     k */
#define LAMBDA 3   /* lambda      λ */

static bool find_set(idx_t depth, idx_t *chosen, idx_t *diff, seq_t *out)
{
    /* All |chosen| = K, check the directed difference counts. */
    if (depth == K) {
        for (idx_t d = 1; d < V; ++d)
            if (diff[d] != LAMBDA)
                return false;
        for (idx_t i = 0; i < K; ++i)
            out[i] = (seq_t)chosen[i];
        return true;
    }

    /* Next element must be larger than the previous to avoid
       duplicate (unordered) representations.  This yields the
       lexicographically smallest solution. */
    idx_t start = chosen[depth - 1] + 1;
    for (idx_t e = start; e < V; ++e) {
        /* Check that adding e does not exceed the allowed λ for any
           directed difference involving e and an already chosen element. */
        bool feasible = true;
        for (idx_t j = 0; j < depth; ++j) {
            idx_t d1 = (e - chosen[j] + V) % V;
            idx_t d2 = (chosen[j] - e + V) % V;
            if (diff[d1] >= LAMBDA || diff[d2] >= LAMBDA) {
                feasible = false;
                break;
            }
        }
        if (!feasible)
            continue;

        /* Update the directed difference counts for the new element. */
        for (idx_t j = 0; j < depth; ++j) {
            idx_t d1 = (e - chosen[j] + V) % V;
            idx_t d2 = (chosen[j] - e + V) % V;
            ++diff[d1];
            ++diff[d2];
        }
        chosen[depth] = e;

        if (find_set(depth + 1, chosen, diff, out))
            return true;           /* Found a complete set */

        /* Backtrack: restore the counts. */
        for (idx_t j = 0; j < depth; ++j) {
            idx_t d1 = (e - chosen[j] + V) % V;
            idx_t d2 = (chosen[j] - e + V) % V;
            --diff[d1];
            --diff[d2];
        }
    }
    return false;                   /* No completion with this depth */
}

seq_t a(idx_t n)
{
    /* Cache for the seven elements of the difference set. */
    static seq_t cache[K];
    static bool initialized = false;

    if (!initialized) {
        idx_t chosen[K];
        idx_t diff[V];
        for (idx_t i = 0; i < V; ++i)
            diff[i] = 0;           /* initialise all difference counters */

        chosen[0] = 0;              /* fix 0 to break cyclic symmetry */
        if (!find_set(1, chosen, diff, cache))
            return (seq_t)0;       /* should not happen for the given parameters */
        initialized = true;
    }

    if (n < 1 || n > K)
        return (seq_t)0;           /* defined only up to K */
    return cache[n - 1];
}

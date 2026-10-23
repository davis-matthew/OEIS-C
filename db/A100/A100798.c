/*
 * A100798  n occurs n times, as early as possible subject to the constraint
 *          that two successive occurrences of n are separated by at least by
 *          n terms.
 * https://oeis.org/A100798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the smallest integer m that can be placed at position n,
 *            where each integer m appears exactly m times, and two successive
 *            occurrences of m are separated by at least m other terms
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We recompute the sequence from scratch for every call,
   keeping only two auxiliary arrays of idx_t: counts and lastpos.
   The arrays are resized on demand. They contain only index‑related
   data, so they remain well‑behaved for both C and C++ builds. */

#include <stdlib.h>   /* malloc, realloc, calloc, free, EXIT_FAILURE */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 */

    /* Allocate auxiliary arrays large enough for at least n indices. */
    size_t sz = (size_t)n + 2;            /* one extra for safety */
    idx_t *counts   = (idx_t *)calloc(sz, sizeof(idx_t));
    idx_t *lastpos  = (idx_t *)calloc(sz, sizeof(idx_t));
    if (counts == NULL || lastpos == NULL) {
        exit(EXIT_FAILURE);
    }

    seq_t result = 0;

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* Find the smallest feasible integer m. */
        for (idx_t m = 1; ; ++m) {
            /* Resize auxiliary arrays if necessary. */
            if ((size_t)m + 1 > sz) {
                sz = ((size_t)m + 1) * 2;
                idx_t *new_counts  = (idx_t *)realloc(counts,  sz * sizeof(idx_t));
                idx_t *new_lastpos = (idx_t *)realloc(lastpos, sz * sizeof(idx_t));
                if (new_counts == NULL || new_lastpos == NULL) {
                    exit(EXIT_FAILURE);
                }
                for (size_t i = (size_t)m + 1; i < sz; ++i) {
                    new_counts[i]  = 0;
                    new_lastpos[i] = 0;
                }
                counts  = new_counts;
                lastpos = new_lastpos;
            }

            /* Check placement constraints. */
            if (counts[m] < m) {
                if (counts[m] == 0 || (pos - lastpos[m] - 1) >= m) {
                    /* Position pos receives value m. */
                    if (pos == n) result = (seq_t)m;
                    ++counts[m];
                    lastpos[m] = pos;
                    break;
                }
            }
        }
    }

    free(counts);
    free(lastpos);
    return result;
}

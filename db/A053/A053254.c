/*
 * A053254  Coefficients of the '3rd-order' mock theta function nu(q).
 * https://oeis.org/A053254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 115, i32 n <= 395, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   recompute_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A053254: Coefficients of the "3rd‑order" mock theta function
 *  nu(q).  a(n) is the coefficient of q^n in the series
 *      nu(q) = Σ_{m≥0} q^{m(m+1)} / ∏_{k=0}^{m} (1+q^{2k+1}).
 *  This implementation computes a(n) exactly for all n ≥ 0 in O(n√n)
 *  time by constructing the inverse product incrementally and
 *  accumulating contributions for each m.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/recompute_up_to.h"

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Static cache keeps all computed terms up to the highest request.
     * The cache is resized only when a larger index is requested. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size of cache */

    if ((size_t)n + 1 > cache_cap) {
        /* Allocate a larger cache, recompute all terms up to n. */
        seq_t *new_cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure – return 0 */

        recompute_up_to(n, new_cache);

        /* Replace old cache.  The old cache is freed automatically
         * after recompute_up_to ensures the result is already stored. */
        free(cache);
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

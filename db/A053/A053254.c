/*
 * A053254  Coefficients of the '3rd-order' mock theta function nu(q).
 * https://oeis.org/A053254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 115, i32 n <= 395, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 996 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Recompute all terms up to Nmax inclusive, storing them in *res. */
static void recompute_up_to(idx_t Nmax, seq_t *res)
{
    size_t sz = (size_t)(Nmax + 1);
    /* initialise result array to zero */
    for (size_t i = 0; i < sz; ++i) {
        res[i] = (seq_t)0;
    }

    /* dp[k] holds the coefficient of q^k in
     *      1 / ∏_{odd j ≤ d} (1+q^j)
     * where d is the current largest odd exponent. */
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) return;                     /* allocation failure – abort silently */
    dp[0] = (seq_t)1;                     /* 1 / 1 = 1 */

    for (idx_t m = 0; ; ++m) {
        idx_t mm = m * (m + 1);           /* exponent of q in the m‑th term   */
        if (mm > Nmax) break;

        idx_t d = 2 * m + 1;              /* the new odd denominator exponent */

        /* Update dp in-place using the identity  
         *     1/(1+q^d) = 1 - q^d + q^{2d} - ...
         * This gives `dp[k] = dp[k] - dp[k-d]` with the left operand
         * already containing the new value of dp[k-d].  The loop
         * direction (increasing k) guarantees that dp[k-d] is already
         * the updated value. */
        for (idx_t k = d; k <= Nmax; ++k) {
            dp[k] -= dp[k - d];
        }

        /* Accumulate the contribution of the m‑th term to all
         * coefficients with index ≥ mm.  We use the updated dp above. */
        for (idx_t k = mm; k <= Nmax; ++k) {
            res[k] += dp[k - mm];
        }
    }

    mem_free(dp);
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Static cache keeps all computed terms up to the highest request.
     * The cache is resized only when a larger index is requested. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size of cache */

    if ((size_t)n + 1 > cache_cap) {
        /* Allocate a larger cache, recompute all terms up to n. */
        seq_t *new_cache = mem_new(seq_t, ((size_t)n + 1));
        if (!new_cache) return (seq_t)0;  /* allocation failure – return 0 */

        recompute_up_to(n, new_cache);

        /* Replace old cache.  The old cache is freed automatically
         * after recompute_up_to ensures the result is already stored. */
        mem_free(cache);
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

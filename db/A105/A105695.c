/*
 * A105695  Expansion of (1-x)*c(x/(1+x)), where c(x) is the g.f. of the
 *          Catalan numbers (A000108).
 * https://oeis.org/A105695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 1. For n ≥ 4: a(n) = [
 *            (n-2)(2n-3)a(n-1) + 3(n-2)(n-3)a(n-2) ] / [ (n+1)(n-3) ]. The
 *            division is exact for all n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types are defined by the build system. */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;    /* Cached values */
    static idx_t computed = 0;  /* Number of computed entries; dp[0..computed-1] valid */
    static idx_t cap = 0;       /* Current allocation size */

    /* First time: allocate minimal space and set the initial terms. */
    if (dp == NULL) {
        cap = 4;                          /* indices 0..3 */
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp) return 0;                /* silently abort on OOM */

        dp[0] = 1;
        dp[1] = 0;
        dp[2] = 0;
        dp[3] = 1;
        computed = 4;
    }

    /* If the requested term is already known, return it immediately. */
    if (n < computed)
        return dp[n];

    /* Grow the cache if necessary. */
    if (cap <= n) {
        idx_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* silently abort on OOM */
        dp = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i <= 3) {                /* base cases already set */
            continue;
        }

        /* Compute the numerator: (i-2)(2i-3)a(i-1) + 3(i-2)(i-3)a(i-2) */
        seq_t term1 = ((seq_t)(i - 2)) * ((seq_t)(2 * i - 3)) * dp[i - 1];
        seq_t term2 = ((seq_t)3) * ((seq_t)(i - 2)) * ((seq_t)(i - 3)) * dp[i - 2];
        seq_t numerator = term1 + term2;

        /* Compute the denominator: (i+1)(i-3) */
        seq_t denominator = ((seq_t)(i + 1)) * ((seq_t)(i - 3));

        dp[i] = numerator / denominator;   /* Integer division – exact */
    }

    computed = n + 1;          /* All indices up to n are now cached */
    return dp[n];
}

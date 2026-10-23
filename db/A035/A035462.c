/*
 * A035462  Number of partitions of n into parts 4k-1.
 * https://oeis.org/A035462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of partitions of n into parts of the form 4k‑1.
 *            G.f.: 1 / ∏_{j≥1} (1 – x^(4j–1))
 * Range:     u8 n <= 78, u16 n <= 190, u32 n <= 535, u64 n <= 1697,
 *            u128 n <= 5873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) – see the comment above for the formula.  */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp   = NULL;   /* array of a(i) for 0 ≤ i ≤ cur_max */
    static idx_t  cur_max = 0;   /* maximum n for which dp[0..cur_max] are valid */
    static idx_t  size   = 0;    /* allocated size of dp */

    /* First call – initialise */
    if (dp == NULL) {
        size   = 1;                /* hold a(0) only */
        dp     = (seq_t *)malloc(sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;   /* allocation failure – unlikely in tests */
        dp[0]  = (seq_t)1;        /* a(0) = 1 */
        cur_max = 0;
    }

    /* If n is beyond the current cache, recompute up to n from scratch. */
    if (n > cur_max) {
        /* Re‑allocate the array to hold values up to n.  */
        seq_t *tmp = (seq_t *)realloc(dp, ((size_t)n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        dp   = tmp;
        size = (size_t)n + 1;

        /* Initialise all entries to zero, then set a(0)=1.  */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Classical unbounded–knapsack DP over all parts 3,7,11,… ≤ n.  */
        for (idx_t part = 3; part <= n; part += 4) {
            for (idx_t j = part; j <= n; ++j) {
                dp[j] += dp[j - part];
            }
        }

        cur_max = n;
    }

    /* If the requested index is already computed, we simply return it. */
    return dp[n];
}

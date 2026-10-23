/*
 * A161232  Number of partitions of n into Catalan numbers A000108 where every
 *          part appears at least 7 times.
 * https://oeis.org/A161232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into Catalan numbers, where
 *            every used part appears at least 7 times
 * Range:     u8 n <= 41, u16 n <= 488, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements a standard coin‑change dynamic programme.
   For each Catalan number p we consider the numbers of ways to use
   at least 7 copies of p.  The recurrence for the auxiliary array
   add[s] (number of partitions that use p at least 7 times and sum
   to s) is

        add[s] = add[s-p] + pre[s-7p]   for s ≥ 7p
   with add[s] = 0 for s < 7p,  pre is the DP totals before this
   coin.  The final DP totals for sums s are

        dp[s] = pre[s] + add[s].

   The algorithm runs in O(n·C) where C is the number of distinct
   Catalan numbers ≤ n, which is very small for n≤10000.  It works
   for all supported seq_t types (uint8_t … arbitrary‑precision
   class).  All allocations are cast to seq_t* where required. */

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Allocate DP tables; casting to seq_t* is allowed in both C and C++. */
    size_t sz = (size_t)n + 1;
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *pre  = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *add  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !pre || !add) {
        free(dp); free(pre); free(add);
        return (seq_t)0;
    }

    /* Base case: empty partition */
    dp[0] = (seq_t)1;

    /* Iterate over all distinct Catalan numbers ≤ n */
    seq_t last = (seq_t)-1;           /* sentinel for duplicates  (1 occurs twice) */
    for (idx_t k = 0;; ++k) {
        seq_t cat = catalan(k);      /* Catalan number (seq_t) */
        if (cat > (seq_t)n) break;   /* larger coins can't be used */
        if (cat == last) continue;   /* skip duplicate 1 */
        last = cat;

        idx_t p = (idx_t)cat;          /* coin value fits in idx_t (≤ n) */

        /* Copy current DP state to pre */
        for (idx_t i = 0; i < sz; ++i) pre[i] = dp[i];

        /* Zero add array (although calloc did it already, keep explicit for clarity) */
        for (idx_t i = 0; i < sz; ++i) add[i] = (seq_t)0;

        /* Compute add[s] for s ≥ 7p */
        idx_t start = 7 * p;
        if (start <= n) {
            for (idx_t s = start; s <= n; ++s) {
                /* add[s-p] may be zero for s-p < start (already zero) */
                seq_t a1 = (s >= p) ? add[s - p] : (seq_t)0;
                seq_t a2 = (s >= 7 * p) ? pre[s - 7 * p] : (seq_t)0;
                add[s] = a1 + a2;
            }
        }

        /* Accumulate into dp */
        for (idx_t i = 0; i < sz; ++i) dp[i] = pre[i] + add[i];
    }

    seq_t result = dp[n];

    free(dp); free(pre); free(add);
    return result;
}

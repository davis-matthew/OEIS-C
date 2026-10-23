/*
 * A035654  Number of partitions of n into parts 7k and 7k+4 with at least one
 *          part of each type.
 * https://oeis.org/A035654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of partitions of n into parts 7k and 7k+4 with at
 *            least one part of each type)
 * Range:     u8 n <= 77, u16 n <= 182, u32 n <= 498, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming using the unbounded knapsack technique.
   For n > 0 a(n) = dpS[n] - dp7[n] - dp4[n],
   where dpS[n] counts all partitions using the two part types,
   dp7[n] counts partitions using only multiples of 7,
   dp4[n] counts partitions using only parts of form 7k+4 (including 4). */

#include <stdlib.h>
#include <stdbool.h>

/* Static arrays for dynamic programming, rebuilt when a larger n is requested. */
static seq_t *dpS  = NULL;   /* partitions using 7k or 7k+4          */
static seq_t *dp7  = NULL;   /* partitions using only multiples of 7 */
static seq_t *dp4  = NULL;   /* partitions using only of form 7k+4   */
static idx_t  maxN = 0;      /* largest n for which the tables are built */

/* Rebuild the tables up to at least N. */
static void
recompute(idx_t N)
{
    /* free old tables if any */
    if (dpS) free(dpS);
    if (dp7) free(dp7);
    if (dp4) free(dp4);

    /* allocate new tables, +1 for zero index */
    size_t sz = (size_t)(N + 1);
    dpS = (seq_t *)calloc(sz, sizeof(seq_t));
    dp7 = (seq_t *)calloc(sz, sizeof(seq_t));
    dp4 = (seq_t *)calloc(sz, sizeof(seq_t));

    /* initialize base case: one way to make sum 0 – empty part set */
    dpS[0] = dp7[0] = dp4[0] = (seq_t)1;

    /* add parts that are multiples of 7 (k >= 1). */
    for (idx_t k = 1; ; ++k) {
        idx_t v = 7 * k;
        if (v > N) break;
        for (idx_t s = v; s <= N; ++s) {
            dpS[s] += dpS[s - v];
            dp7[s] += dp7[s - v];
        }
    }

    /* add parts of the form 7k+4 (k >= 0, so the part 4 is included). */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 7 * k + 4;
        if (v > N) break;
        for (idx_t s = v; s <= N; ++s) {
            dpS[s] += dpS[s - v];
            dp4[s] += dp4[s - v];
        }
    }

    maxN = N;
}

/* Return a(n).  For n < 1 the result is 0. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (n > maxN)
        recompute(n);

    /* partitions that use both parts types are counted in dpS but not in dp7 or dp4. */
    return dpS[n] - dp7[n] - dp4[n];
}

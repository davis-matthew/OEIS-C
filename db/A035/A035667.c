/*
 * A035667  Number of partitions of n into parts 7k+3 and 7k+5 with at least
 *          one part of each type.
 * https://oeis.org/A035667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P[n] – P3[n] – P5[n] where P[n] = #partitions of n using
 *            parts of form 7k+3 or 7k+5, P3[n] = #partitions of n using only
 *            parts 7k+3, P5[n] = #partitions of n using only parts 7k+5. All
 *            three counts are taken modulo the ordinary integer arithmetic of
 *            the type seq_t. The function returns a(i...
 * Range:     u8 n <= 67, u16 n <= 161, u32 n <= 457, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_STATIC_N 10000          /* up to which we keep static tables */

static seq_t dp3_static[MAX_STATIC_N + 1];
static seq_t dp5_static[MAX_STATIC_N + 1];
static seq_t dpAll_static[MAX_STATIC_N + 1];

/* Compute the three DP tables up to N and store the results in the
   provided arrays.  Each array must have at least N+1 elements. */
static void compute_partitions(idx_t N,
                               seq_t *dp3,
                               seq_t *dp5,
                               seq_t *dpAll)
{
    /* initialize (dp[0] = 1, others 0) */
    for (idx_t i = 1; i <= N; ++i) {
        dp3[i]   = (seq_t)0;
        dp5[i]   = (seq_t)0;
        dpAll[i] = (seq_t)0;
    }
    dp3[0]   = (seq_t)1;
    dp5[0]   = (seq_t)1;
    dpAll[0] = (seq_t)1;

    /* parts of the form 7k+3 */
    for (idx_t v = 3; v <= N; v += 7) {
        for (idx_t s = v; s <= N; ++s) {
            dp3[s]   += dp3[s - v];
            dpAll[s] += dpAll[s - v];
        }
    }

    /* parts of the form 7k+5 */
    for (idx_t v = 5; v <= N; v += 7) {
        for (idx_t s = v; s <= N; ++s) {
            dp5[s]   += dp5[s - v];
            dpAll[s] += dpAll[s - v];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (n <= MAX_STATIC_N) {
        compute_partitions(n,
                           dp3_static,
                           dp5_static,
                           dpAll_static);

        seq_t total = dpAll_static[n];
        seq_t only3 = dp3_static[n];
        seq_t only5 = dp5_static[n];
        return total - only3 - only5;
    }

    /* For n larger than the static limit allocate arrays */
    size_t sz = (size_t)n + 1;
    seq_t *dp3   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp5   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dpAll = (seq_t *)calloc(sz, sizeof(seq_t));

    if (!dp3 || !dp5 || !dpAll) {
        /* memory allocation failed – abort */
        exit(1);
    }

    /* set zero/one manually since calloc already did zero */
    dp3[0]   = (seq_t)1;
    dp5[0]   = (seq_t)1;
    dpAll[0] = (seq_t)1;

    compute_partitions(n, dp3, dp5, dpAll);

    seq_t result = dpAll[n] - dp3[n] - dp5[n];

    free(dp3);
    free(dp5);
    free(dpAll);

    return result;
}

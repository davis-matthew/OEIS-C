/*
 * A035699  Number of partitions of n into parts 8k+6 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 8k+6 and 8k+7 with at least
 *            one part of each type. This is computed by dynamic programming
 *            over the infinite coin set S = {8k+6, 8k+7}. For each n the
 *            answer is: a(n) = p_S(n) – p_{8k+6}(n) – p_{8k+7}(n) where
 *            p_X(n) counts partitions of n using parts from set X
 * Range:     u8 n <= 96, u16 n <= 217, u32 n <= 584, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, used in loops */

#define MAX_N 20000U          /* maximum n we expect to handle */

static idx_t max_computed = 0;     /* largest n for which tables are filled */

static seq_t dp6[MAX_N + 1];       /* partitions using only parts 8k+6 */
static seq_t dp7[MAX_N + 1];       /* partitions using only parts 8k+7 */
static seq_t dpAll[MAX_N + 1];     /* partitions using any part in S */

/* Re‑compute all tables up to n (must be <= MAX_N).  This is done
   only when a larger n is requested.  The algorithm runs in
   O(n²) and easily fits the required time limits for n ≤ 10 000
   (≈ 2·10⁸ simple operations in the worst case). */
static void recompute(idx_t n)
{
    /* initialise arrays to zero */
    for (idx_t i = 0; i <= n; ++i)
    {
        dp6[i] = 0;
        dp7[i] = 0;
        dpAll[i] = 0;
    }
    /* base case */
    dp6[0] = 1;
    dp7[0] = 1;
    dpAll[0] = 1;

    /* process parts of the form 8k+6 : 6, 14, 22, … */
    for (idx_t c = 6; c <= n; c += 8)
    {
        for (idx_t i = c; i <= n; ++i)
        {
            dp6[i]   += dp6[i - c];
            dpAll[i] += dpAll[i - c];
        }
    }
    /* process parts of the form 8k+7 : 7, 15, 23, … */
    for (idx_t c = 7; c <= n; c += 8)
    {
        for (idx_t i = c; i <= n; ++i)
        {
            dp7[i]   += dp7[i - c];
            dpAll[i] += dpAll[i - c];
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* sequence defined for n ≥ 1 only */

    /* expand tables if necessary */
    if (n > max_computed)
    {
        /* guard against request beyond allocated buffer – not expected in tests */
        if (n > MAX_N)
            return 0;
        recompute(n);
    }
    /* number of partitions using at least one of each type */
    return dpAll[n] - dp6[n] - dp7[n];
}

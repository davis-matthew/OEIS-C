/*
 * A025280  Complexity of n: number of 1's required to build n using +, *, and
 *          ^.
 * https://oeis.org/A025280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 939, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025280: Complexity of n – minimal number of 1's needed
 * to build n using +, *, and ^.  Computation is performed by dynamic
 * programming.  For n ≥ 1, a(1) = 1.  The algorithm evaluates each
 * value as the minimum of:
 *
 *   dp[a] + dp[n–a]          (addition)
 *   dp[a] + dp[n/a]          (multiplication, when a | n)
 *   dp[r] + dp[e]            (exponentiation, r^e = n)
 *
 * where dp[x] is the minimal cost for x.  dp[1] = 1.
 *
 * The implementation respects the constraints on types: all
 * arithmetic on the dp array uses seq_t; indices are idx_t.
 * Standard headers are used, and dynamic allocation is the only
 * global state. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Dynamic programming table and current maximal index computed. */
static seq_t *dp = NULL;
static idx_t max_n = 0;

/* Ensure that dp[1..n] have been filled. */
static void _ensure_dp(idx_t n)
{
    if (max_n >= n)
        return;

    /* Allocate/extend the table. */
    seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!new_dp)
        exit(EXIT_FAILURE);
    dp = new_dp;

    /* Initialise the base cases if this is the first allocation. */
    if (max_n == 0) {
        dp[0] = 0;      /* unused */
        dp[1] = 1;      /* a(1) = 1 */
        max_n  = 1;
    }

    /* Compute values for all indices > max_n up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t best = (seq_t)i;   /* worst case: use i ones */

        /* Addition decompositions. */
        for (idx_t a = 1; a <= i / 2; ++a) {
            seq_t cand = dp[a] + dp[i - a];
            if (cand < best)
                best = cand;
        }

        /* Multiplication decompositions. */
        idx_t limit = (idx_t)sqrt((double)i);
        for (idx_t a = 2; a <= limit; ++a) {
            if (i % a == 0) {
                seq_t cand = dp[a] + dp[i / a];
                if (cand < best)
                    best = cand;
            }
        }

        /* Exponentiation decompositions. */
        /* Maximum possible exponent (floor(log2(i))) */
        idx_t temp  = i;
        idx_t emax  = 0;
        while (temp > 1) {
            temp >>= 1;
            ++emax;
        }
        for (idx_t e = 2; e <= emax; ++e) {
            /* Estimate maximum base r such that r^e <= i. */
            double root_d = pow((double)i, 1.0 / e);
            idx_t rmax = (idx_t)(root_d + 1.0);
            if (rmax < 2)
                continue;

            for (idx_t r = 2; r <= rmax; ++r) {
                /* Check whether r^e == i. */
                idx_t p = 1;
                bool match = true;
                for (idx_t k = 0; k < e; ++k) {
                    if (p > i / r) {
                        match = false;
                        break;
                    }
                    p *= r;
                }
                if (!match || p != i)
                    continue;

                seq_t cand = dp[r] + dp[e];
                if (cand < best)
                    best = cand;
            }
        }

        dp[i] = best;
    }

    max_n = n;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but harmless */

    _ensure_dp(n);
    return dp[n];
}

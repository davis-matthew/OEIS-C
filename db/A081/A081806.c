/*
 * A081806  Least positive number that can be written using prime factors of n
 *          (with repetition) and the operations add and subtract.
 * https://oeis.org/A081806
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_multiset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081806 – Least positive number that can be written using prime
    factors of n (with repetition) and the operations add and subtract.
    
    Formula: a(n) = min_{S ⊆ multiset_of_prime_factors(n)}
                 | (∑_{p∈S} p) – (∑_{p∉S} p) | ,
                 with a(n) > 0 and a(p)=p for prime n.
    The implementation factors n, builds the multiset of prime factors,
    and then uses a 0/1 knapsack DP over the factor sums to find the
    minimal positive difference.
*/

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_multiset.h"

/* ----------------------------------------------------------------------
 * DP bitset for subset sums.
 * MAX_SUM must be at least the maximum possible sum of prime factors
 * for the input range. 20000 comfortably covers all n ≤ 10000.
 * ---------------------------------------------------------------------- */
#define MAX_SUM 20000
static bool dp[MAX_SUM + 1];

/* ----------------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The offset for this sequence is 2; behaviour below 2 is irrelevant. */
    if (n < 2)
        return (seq_t)n;

    unsigned long long facts[64];   /* buffer for at most 64 prime factors */
    size_t fcnt;
    idx_t sum;

    factor_multiset(n, facts, &fcnt, &sum);

    if (sum > MAX_SUM)              /* sum too large for the static DP buffer */
        return (seq_t)0;            /* undefined for these inputs – should not occur */

    /* Initialise DP bitset */
    for (size_t i = 0; i <= (size_t)sum; ++i)
        dp[i] = false;
    dp[0] = true;

    /* 0/1 knapsack on the multiset of prime factors */
    for (size_t i = 0; i < fcnt; ++i) {
        size_t v = (size_t)facts[i];
        for (size_t s = (size_t)sum; s >= v; --s) {
            if (dp[s - v])
                dp[s] = true;
        }
    }

    /* Find minimal positive difference */
    idx_t best = sum;       /* upper bound: using only one side gives difference sum */
    for (size_t s = 0; s <= (size_t)sum; ++s) {
        if (!dp[s])
            continue;
        idx_t diff = (sum >= 2 * (idx_t)s) ? (sum - 2 * (idx_t)s) : (2 * (idx_t)s - sum);
        if (diff > 0 && diff < best)
            best = diff;
    }

    return (seq_t)best;
}

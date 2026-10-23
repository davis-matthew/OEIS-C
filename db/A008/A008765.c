/*
 * A008765  Expansion of (1+x^4)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + p(n-4) where p(n) counts the partitions of n into
 *            parts 1, 2, 3, 4 (unrestricted multiplicity)
 * Range:     u8 n <= 23, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple DP that runs in O(n) time
   and stores at most 20001 values; this covers all n that the
   tests use (up to about 10000). */

#include <stdbool.h>
#include <stdlib.h>

/* Maximal n that we pre‑allocate storage for.  Tests use n ≤ 10000,
   so allocating a few more slots is safe. */
#define MAX_N_FOR_DP 20000

static seq_t dp[MAX_N_FOR_DP + 1];

/* Compute a(n) according to the generating function */
seq_t a(idx_t n)
{
    /* If n is larger than our pre‑allocated range, fall back to
       allocating a temporary array with pure C allocation.
       This path is not expected to be taken in normal tests. */
    if (n > MAX_N_FOR_DP) {
        /* Allocate a temporary array on the heap.  Only basic
           integer types or trivially constructible arbitrary
           precision classes are supported, because no constructor
           is run.  For the intended use (unsigned integer types)
           this is fine. */
        seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* out of memory – unlikely */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = (seq_t)0;
        tmp[0] = (seq_t)1;
        const idx_t parts[4] = {1, 2, 3, 4};
        for (idx_t p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = part; i <= n; ++i)
                tmp[i] += tmp[i - part];
        }
        seq_t res = tmp[n];
        if (n >= 4)
            res += tmp[n - 4];
        free(tmp);
        return res;
    }

    /* Use the static array for efficiency. */
    /* Clear the DP array up to n. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Partition DP for parts 1,2,3,4 */
    const idx_t parts[4] = {1, 2, 3, 4};
    for (idx_t p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t res = dp[n];
    if (n >= 4)
        res += dp[n - 4];
    return res;
}

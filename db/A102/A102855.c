/*
 * A102855  Minimal number of distinct nonzero tetrahedral numbers needed to
 *          represent n, or -1 if no such representation is possible.
 * https://oeis.org/A102855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = minimal number of distinct non‑zero tetrahedral numbers
 *            T_k = k(k+1)(k+2)/6 whose sum equals n; a(n) = -1 if no such
 *            representation exists
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)-1;           /* below the offset */

    /*--------------------------------------------------------------------
     *  Build the list of all tetrahedral numbers ≤ n.
     *--------------------------------------------------------------------*/
    size_t cap = 8;
    idx_t *tetra = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!tetra)
        return (seq_t)-1;
    size_t len = 0;

    for (uint64_t k = 1; ; ++k) {
        unsigned __int128 t128 = (unsigned __int128)k * (k + 1ULL) * (k + 2ULL) / 6ULL;
        if (t128 > n)
            break;
        if (len == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(tetra, cap * sizeof(idx_t));
            if (!tmp) { free(tetra); return (seq_t)-1; }
            tetra = tmp;
        }
        tetra[len++] = (idx_t)t128;
    }

    /*--------------------------------------------------------------------
     *  0/1 Knapsack DP – minimise number of terms.
     *--------------------------------------------------------------------*/
    int *dp = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (!dp) { free(tetra); return (seq_t)-1; }

    const int INF = INT_MAX / 2;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = INF;
    dp[0] = 0;

    for (size_t i = 0; i < len; ++i) {
        idx_t val = tetra[i];
        for (idx_t s = n; s >= val; --s) {
            int prev = dp[s - val];
            if (prev != INF && prev + 1 < dp[s])
                dp[s] = prev + 1;
        }
    }

    int result = dp[(size_t)n];
    free(tetra);
    free(dp);

    if (result == INF)
        return (seq_t)-1;

    return (seq_t)result;
}

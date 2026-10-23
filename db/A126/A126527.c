/*
 * A126527  Number of base 32 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑32 n‑digit strings in which any two
 *            adjacent digits differ by at most four. * a(0)=1, a(1)=32. For
 *            n≥1 let A be the 32×32 adjacency matrix with Aij=1 if |i-j|≤4
 *            else 0. * Then a(n)=sum_{i,j} (A^(n-1))ij. The DP below iterates
 *            over the layers of this graph
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

static seq_t *ans = NULL;
static size_t ansCap = 0;
static size_t ansUsed = 0;
static seq_t dp[2][32];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;        /* a(0) = 1 */

    /* first call initialisation */
    if (!initialized) {
        /* start with capacity 128, extend if needed */
        ansCap = 128;
        ans = seq_alloc_array(ansCap);
        ans[0] = (seq_t)1;          /* a(0) */
        ans[1] = (seq_t)32;         /* a(1) */
        ansUsed = 2;

        /* dp for length 1: each digit can occur once */
        for (int i = 0; i < 32; ++i)
            dp[0][i] = (seq_t)1;

        initialized = 1;
    }

    /* extend precomputation up to max(n, current) */
    while (ansUsed <= (size_t)n) {
        size_t curLen = ansUsed;                 /* length just computed */
        size_t prevIdx = curLen % 2;             /* index of distribution for current length */
        size_t nextIdx = 1 - prevIdx;            /* where to store distribution for next length */

        /* zero next distribution */
        for (int d = 0; d < 32; ++d)
            dp[nextIdx][d] = (seq_t)0;

        /* propagate current counts to neighbours */
        for (int j = 0; j < 32; ++j) {
            seq_t ways = dp[prevIdx][j];
            if (ways == (seq_t)0) continue;
            int start = j - 4; if (start < 0) start = 0;
            int end   = j + 4; if (end   > 31) end   = 31;
            for (int d = start; d <= end; ++d)
                dp[nextIdx][d] += ways;
        }

        /* sum all paths of new length */
        seq_t sum = (seq_t)0;
        for (int d = 0; d < 32; ++d)
            sum += dp[nextIdx][d];

        /* ensure answer array has space */
        if (ansUsed + 1 > ansCap) {
            size_t newCap = ansCap * 2;
            seq_t *tmp = seq_realloc(ans, ansCap, newCap);
            ans = tmp;
            ansCap = newCap;
        }

        ans[ansUsed] = sum;
        ansUsed++;
        /* dp[nextIdx] now represents distribution for length curLen+1 */
    }

    return ans[n];
}

/*
 * A086437  Triangle of number C(3; m,k) of arrangements of k objects of one
 *          type and m of another such that no 3-run occurs.
 * https://oeis.org/A086437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts arrangements of m objects of one type and k of another
   (binary strings with m zeros and k ones) that avoid three equal
   consecutive symbols.  The sequence a(n) is the flattening of the
   triangle with rows of length 2m+3, rows indexed by m ≥ 0 and
   columns k = 0 … 2m+2.  a(row_start(m)+k) = C3(m,k).  */

#include <stdbool.h>

#define MAXM 200
#define MAXK 402

/* Memoised counts for subproblems
   dp[m][k][last][len-1]
   last  : 0 for zero, 1 for one
   len-1 : 0 for run length 1, 1 for run length 2 */
static seq_t dp[MAXM + 1][MAXK + 1][2][2];
static bool vis[MAXM + 1][MAXK + 1][2][2];

/* Memoised totals without a specified leading symbol.
   total_cache[m][k] stores C3(m,k). */
static seq_t total_cache[MAXM + 1][MAXK + 1];
static bool total_vis[MAXM + 1][MAXK + 1];

/* Recursively counts arrangements for given counts of zeros (m)
   and ones (k), with the last placed symbol and the length of the
   current run. */
static seq_t rec(int m, int k, int last, int len)
{
    if (m == 0 && k == 0)
        return (seq_t)1;

    int lidx = len - 1;                  /* 0 for run length 1, 1 for run length 2 */
    if (vis[m][k][last][lidx])
        return dp[m][k][last][lidx];

    vis[m][k][last][lidx] = true;
    seq_t res = (seq_t)0;

    if (last == 0) {                     /* last symbol was 0 */
        if (m > 0 && len < 2)            /* place another 0 */
            res += rec(m - 1, k, 0, len + 1);
        if (k > 0)                        /* place a 1 */
            res += rec(m, k - 1, 1, 1);
    } else {                              /* last symbol was 1 */
        if (k > 0 && len < 2)            /* place another 1 */
            res += rec(m, k - 1, 1, len + 1);
        if (m > 0)                        /* place a 0 */
            res += rec(m - 1, k, 0, 1);
    }

    dp[m][k][last][lidx] = res;
    return res;
}

/* Total count for (m,k) without specifying the first symbol.
   Handles the empty-string case (m=k=0) specially. */
static seq_t total_count(int m, int k)
{
    if (total_vis[m][k])
        return total_cache[m][k];

    seq_t res = (seq_t)0;
    if (m == 0 && k == 0) {
        res = (seq_t)1;                   /* empty sequence */
    } else {
        if (m > 0)                          /* start with 0 */
            res += rec(m - 1, k, 0, 1);
        if (k > 0)                          /* start with 1 */
            res += rec(m, k - 1, 1, 1);
    }

    total_vis[m][k] = true;
    total_cache[m][k] = res;
    return res;
}

/* Public entry point used by the test harness. */
seq_t a(idx_t n)
{
    /* Determine the row index m such that row_start(m) ≤ n < row_start(m+1). */
    idx_t m = 0;
    while ((m + 1) * (m + 1) + 2 * (m + 1) <= n)
        ++m;

    idx_t start = m * m + 2 * m;   /* first index of this row */
    idx_t k = n - start;           /* column within the row */

    /* The required m and k are guaranteed to be < MAXM and < MAXK
       for the range of n considered by the tests. */
    int mi = (int)m;
    int ki = (int)k;
    return total_count(mi, ki);
}

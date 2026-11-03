/*
 * A228617  T(n,k) is the number of s in {1,...,n}^n having shortest run with
 *          the same value of length k; triangle T(n,k), n>=0, 0<=k<=n, read
 *          by rows.
 * https://oeis.org/A228617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the flattened triangle T(n,k) of OEIS
 *            A228617. T(n,k) = n * (W(n,k) - W(n,k+1)) where W(n,m) = Σ_{r}
 *            binom(n-1,r-1)*(n-1)^(r-1) over all compositions of n with parts
 *            ≥ m. This implementation uses a dynamic programming calculation
 *            of W(n,m)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 378, big any n
 * Validated: standard, pass, 82 terms through n = 9919 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     /* for memset */
#include "../utils/mem.h"

/* @brief  Weighted compositions count W(n,min)
 * @param  n   word length / alphabet size
 * @param  min minimum value of a part (>=1)
 * @return  W(n,min) as seq_t; 0 if min>n
 */
static seq_t w_min(idx_t n, idx_t min)
{
    if (min > n) return (seq_t)0;
    idx_t N = n;
    /* allocate array and clear it; dp[0] will be set to 1 explicitly */
    seq_t *dp = mem_new(seq_t, (size_t)(N + 1));
    if (!dp) return (seq_t)0;
    mem_set(dp, 0, (size_t)(N + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;                /* empty composition (0 parts, weight 1) */

    seq_t n_minus_1 = (seq_t)n - (seq_t)1;

    for (idx_t i = 1; i <= N; ++i) {
        seq_t val = 0;
        if (i >= min) val = (seq_t)1;            /* single part i */
        if (i > min) {                           /* compositions ending with a part ≥min */
            for (idx_t p = min; p <= i - 1; ++p) {
                idx_t r = i - p;                 /* remaining length (r ≥1) */
                val += n_minus_1 * dp[r];
            }
        }
        dp[i] = val;
    }
    seq_t res = dp[N];
    mem_free(dp);
    return res;
}

/* @brief  Count of words of length n over alphabet {1..n} with
 *         shortest run length exactly k.
 * @param  n  word length / alphabet size
 * @param  k  desired shortest run length (0 ≤ k ≤ n)
 * @return   T(n,k) as seq_t
 */
static seq_t count_T(idx_t n, idx_t k)
{
    if (k == 0 || k > n)          /* no run of length 0; impossible if k>n */
        return (seq_t)0;
    seq_t w_k  = w_min(n, k);
    seq_t w_k1 = w_min(n, k + 1);
    seq_t diff = (w_k > w_k1) ? w_k - w_k1 : (seq_t)0;
    return (seq_t)n * diff;
}

/* @brief  Flattened triangle value a(n)
 * @param  n  index (offset 0)
 * @return   a(n) as seq_t
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* empty word */
        return (seq_t)1;

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;                    /* column in the row */

    /* For k == 0 the shortest run cannot be 0 (except the empty row) */
    if (k == 0)
        return (seq_t)0;

    return count_T(r, k);
}

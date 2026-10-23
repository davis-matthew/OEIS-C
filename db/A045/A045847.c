/*
 * A045847  Matrix whose (i,j)-th entry is number of representations of j as a
 *          sum of i squares of nonnegative integers; read by diagonals.
 * https://oeis.org/A045847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 357, u32 n <= 1142, u64 n <= 4035,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A045847
    a(n) = number of representations of j as a sum of i squares,
    where (i,j) are obtained from n by unfolding the anti‑diagonals
    of the infinite matrix M with M[i][j] = #solutions of
    x1² + … + xi² = j  (nonnegative integers).
    Diagonal d satisfies d(d+1)/2 ≤ n < (d+1)(d+2)/2,
    i = d - (n - d(d+1)/2),  j = n - d(d+1)/2.
    The function below computes a(n) for any n ≥ 0, on demand,
    using a dynamic DP array that is resized only when necessary.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declarations for idx_t and seq_t are provided by the build. */

static seq_t *dp = NULL;          /* 1‑D array holding the DP table. */
static idx_t  max_d = 0;          /* current maximum d we have allocated for. */

/* ------------------------------------------------------------------
   Find the anti‑diagonal index d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2.
   ------------------------------------------------------------------*/
static idx_t find_diag(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* extend hi until it is definitely large enough */
    while ((hi + 1) * (hi + 2) / 2 <= n) hi <<= 1;
    /* binary search */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n) lo = mid;
        else                         hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------
   Ensure dp can store a table for all i,j with 0 ≤ i,j ≤ d.
   The table is stored as a 1‑D array of length (d+1)*(d+1),
   indexed by i*(d+1)+j.
   ------------------------------------------------------------------*/
static void ensure_dp(idx_t d)
{
    if (d <= max_d) return; /* already allocated */

    /* size needed for new capacity */
    size_t new_cap = (size_t)(d + 1) * (d + 1);

    /* realloc the array, casting appropriately for C++ */
#ifdef __cplusplus
    dp = static_cast<seq_t*>(realloc(dp, new_cap * sizeof(seq_t)));
#else
    dp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
#endif
    max_d = d;
}

/* ------------------------------------------------------------------
   Compute the value a(n).
   ------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;  /* a(0) = 1 */

    /* locate diagonal and coordinates */
    idx_t d      = find_diag(n);
    idx_t pos    = n - d * (d + 1) / 2;   /* 0 ≤ pos ≤ d */
    idx_t i      = d - pos;               /* row index */
    idx_t j      = pos;                   /* column index */

    if (i == (idx_t)0) return (j == (idx_t)0) ? (seq_t)1 : (seq_t)0;

    /* make sure we have space for table up to d */
    ensure_dp(d);

    /* base row i = 0 */
    dp[0] = (seq_t)1;
    for (idx_t col = 1; col <= d; ++col)
        dp[col] = (seq_t)0;

    /* DP: compute rows 1 … d */
    for (idx_t row = 1; row <= d; ++row) {
        for (idx_t col = 0; col <= d; ++col) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; ; ++k) {
                idx_t sq = k * k;
                if (sq > col) break;
#if defined(__USE_MINGW_GNUC__) && !defined(__cpp_int128)
                /* use 128‑bit to avoid overflow on very large indices */
                sum += dp[(row - 1) * (d + 1) + (col - sq)];
#else
                sum += dp[(row - 1) * (d + 1) + (col - sq)];
#endif
            }
            dp[row * (d + 1) + col] = sum;
        }
    }

    return dp[i * (d + 1) + j];
}

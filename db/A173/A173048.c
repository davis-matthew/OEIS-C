/*
 * A173048  Triangle T(n, k) = [x^k](p(x, n, q)) where p(x,n,q) =
 *          Product_{j=1..n} (x + q^j) + Product_{j=1..n} (x*q^j + 1), p(x, 0,
 *          q) = 1, and q = 2, read by rows.
 * https://oeis.org/A173048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 65, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A173048 – Triangle T(n,k) with q = 2, read by rows.
 * T(r,k) = [x^k]( ∏_{j=1..r}(x+2^j) + ∏_{j=1..r}(x*2^j+1) ),
 * where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n – r*(r+1)/2.
 * For r = 0 we have a(0) = 1 (the empty product gives 1, not 2).
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* DP array for S(r,t) */
static idx_t dp_cap = 0;      /* Current capacity of dp */

/*
 * Return a(n) for any n ≥ 0.
 * The function is memoised only for the DP array; each call rebuilds
 * the row freshly.  This is O(r^2) where r ≈ √(2n), which is fine
 * for n up to 10 000.
 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((((r + 1) * (r + 2)) / 2) <= n)
        ++r;

    /* Special case for the first row. */
    if (r == 0)
        return (seq_t)1;

    /* Column within the row. */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Ensure dp has enough space. */
    idx_t need = r + 1;
    if (dp_cap < need) {
        seq_t *tmp = (seq_t *)realloc(dp, need * sizeof(seq_t));
        if (!tmp) {                    /* Allocation failed. */
            free(dp);
            dp = NULL;
            dp_cap = 0;
            return (seq_t)0;
        }
        dp = tmp;
        dp_cap = need;
    }

    /* Initialise dp. */
    dp[0] = (seq_t)1;
    for (idx_t t = 1; t < need; ++t)
        dp[t] = (seq_t)0;

    /* Compute S(r,t) by DP: dp[t] = Σ_{|S|=t} 2^{sum S}. */
    seq_t power2 = (seq_t)1;          /* 2^0 */
    for (idx_t j = 1; j <= r; ++j) {   /* j runs from 1 to r */
        power2 = power2 * (seq_t)2;    /* power2 = 2^j */
        for (idx_t t = j; t > 0; --t)
            dp[t] = dp[t] + dp[t - 1] * power2;
    }

    /* T(r,k) = S(r,k) + S(r,r-k). */
    return dp[k] + dp[r - k];
}

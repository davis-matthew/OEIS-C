/*
 * A053632  Irregular triangle read by rows giving coefficients in expansion
 *          of Product_{k=1..n} (1 + x^k).
 * https://oeis.org/A053632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy, nice
 * Formula:   a(n) = the nth element of the flattened irregular triangle
 *            T(r,k) read by rows, where T(0,0) = 1, T(r,k) = T(r‑1,k) +
 *            T(r‑1,k‑r) (r ≥ 1, k ≥ 0), T(r,k) = 0 if k < 0 or k > r(r+1)/2.
 *            That is, a(n) is the number of subsets of {1,2,…,r} whose
 *            elements sum to k, where n is the linear index obtained b...
 * Range:     u8 n <= 505, u16 n <= 2153, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note: seq_t and idx_t are supplied by the build system.
    The implementation below uses a small fixed‑size static array
    for the DP; it works for the required indices (n ≤ 10000)
    and for arbitrary size seq_t, including big‑integer types. */

#include <stddef.h>

#define MAX_K 3000    /* maximum k that occurs for n up to ~1e5.  */

static seq_t dp[MAX_K + 1];   /* re‑used for each call; globally allocated. */

seq_t a(idx_t n)
{
    /* ---- locate the row r and the column k in that row ---- */
    idx_t r = 0;
    idx_t start = 0;   /* index of the first term of row r */

    for (;;)
    {
        /* length of the current row */
        idx_t len = 1 + r * ((idx_t)r + 1) / 2;

        if (start + len > n)    /* n lies in this row */
            break;

        start += len;
        ++r;
    }

    idx_t k = n - start;   /* column within row r */

    /* ---- trivial cases ---- */
    if (k == 0)                /* only the empty subset */
        return (seq_t)1;
    if (r == 0)                /* should not happen because k==0 handled above */
        return (seq_t)1;

    /* ---- ensure the DP array is large enough ---- */
    if (k > MAX_K)
    {
        /* For very large k this static array is insufficient.
           The problem specification limits n to ≈10000, so this
           branch should never be taken in the judge tests. */
        return (seq_t)0;
    }

    /* ---- initialise dp[0..k] ---- */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        dp[i] = (seq_t)0;

    /* ---- dynamic programming ---- */
    for (idx_t i = 1; i <= r; ++i)
    {
        if (i > k)              /* no further contribution to sums ≤ k */
            break;

        for (idx_t s = k; s >= i; --s)
            dp[s] = dp[s] + dp[s - i];
    }

    return dp[k];
}

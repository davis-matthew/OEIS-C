/*
 * A105196  Triangle read by rows: T(n,k) = |Stirling1(n,k)| * k^k, for 1 <= k
 *          <= n.
 * https://oeis.org/A105196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n‑th) entry of the triangle T(r,c) with *
 *            T(r,c)=|S1(r,c)|*c^c (unsigned Stirling numbers of the first
 *            kind), * read row by row (row r, column c); * the mapping is:
 *            find the minimal r such that r*(r+1)/2 >= n, * then c = n -
 *            r*(r-1)/2
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   power_kk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are supplied by the build.  The function
 * works for any n>=1.  Values that fit into seq_t are computed exactly. */

#include <stdbool.h>
#include "../utils/power_kk.h"

#define MAX_R 200                     /* enough for n up to >20000   */

static seq_t Stirling[MAX_R+1][MAX_R+1] = {0};
static idx_t max_precomputed = 0;

/* Find minimal r such that r*(r+1)/2 >= n (1‑based).  Uses integer binary search. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1u) / 2u < n)
        hi <<= 1u;
    while (lo + 1u < hi)
    {
        idx_t mid = lo + (hi - lo) / 2u;
        if ((unsigned __int128)mid * (mid + 1u) / 2u >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1, n==0 unused */

    idx_t r = find_row(n);          /* row (1‑based) */
    unsigned __int128 prev128 = (unsigned __int128)r * (r - 1u) / 2u;
    idx_t prev = (idx_t)prev128;    /* elements before this row      */
    idx_t k = n - prev;             /* column within the row (1‑based) */

    /* Ensure Stirling numbers are computed up to row r.  */
    if (max_precomputed < r)
    {
        if (max_precomputed == 0u)
            Stirling[0][0] = 1;     /* base case */

        for (idx_t i = max_precomputed + 1u; i <= r; ++i)
        {
            Stirling[i][0] = 0;     /* S(n,0)=0 for n>0              */
            for (idx_t j = 1u; j <= i; ++j)
            {
                seq_t term1 = Stirling[i - 1u][j - 1u];
                seq_t term2 = Stirling[i - 1u][j] * (seq_t)(i - 1u);
                Stirling[i][j] = term1 + term2;
            }
        }
        max_precomputed = r;
    }

    seq_t stir = Stirling[r][k];      /* |S1(r,k)| */
    seq_t pk   = power_kk((seq_t)k, k); /* k^k      */
    return stir * pk;
}

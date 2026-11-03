/*
 * A233321  Triangle read by rows: T(n,k) = number of palindromic partitions
 *          of n in which the largest part is equal to k, 1 <= k <= n.
 * https://oeis.org/A233321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 708, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 121 terms through n = 1259 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A233321:  Triangle read by rows.
 *  a(n) is the nth term of the triangle, i.e. the element at row r, column k
 *  where r is the smallest integer with r(r+1)/2 ≥ n, and k = n – (r-1)r/2.
 *  The value is T(r,k) = number of palindromic partitions of r in which
 *  the largest part equals k.
 *
 *  A partition is palindromic iff the multiplicity of each part is even
 *  except possibly one part.  Let F(r,k) be the number of palindromic
 *  partitions of r with largest part ≤ k.  Then
 *      T(r,k) = F(r,k) – F(r,k-1).
 *
 *  F(r,k) can be computed as
 *      F(r,k) = Σ_{c=0..k, (r-c) even} p((r-c)/2 , k),
 *  where p(m,k) is the number of ordinary partitions of m using parts
 *  ≤ k.  To obtain p(m,k) we use the standard 1‑D DP over part size.
 *
 *  This implementation follows that formula and uses only integer
 *  arithmetic.  It allocates a small DP array via malloc, which is safe
 *  for the sizes considered (r ≤ ~150 for n ≤ 10000).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------ */
static seq_t total_with_limit(idx_t r, idx_t k)
{
    /* Compute F(r,k): number of palindromic partitions of r with largest part ≤ k */
    idx_t M = r >> 1;                     /* floor(r/2) */
    size_t sz = (size_t)(M + 1);
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) return (seq_t)0;             /* allocation failure → 0 (should not happen) */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= M; ++i) dp[i] = (seq_t)0;

    for (idx_t v = 1; v <= k; ++v) {
        for (idx_t s = v; s <= M; ++s) {
            dp[s] += dp[s - v];
        }
    }

    seq_t total = (seq_t)0;
    for (idx_t c = 0; c <= k; ++c) {
        idx_t diff = r - c;
        if ((diff & 1) == 0) {
            idx_t m = diff >> 1;
            total += dp[m];
        }
    }

    mem_free(dp);
    return total;
}

/* ------------------------------------------------------------ */
/* External function required by the judge */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (true) {
        idx_t tri = r * (r + 1) / 2;
        if (tri >= n)
            break;
        ++r;
    }

    /* Column k in that row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;              /* 1 <= k <= r */

    seq_t total_k   = total_with_limit(r, k);
    seq_t total_km1 = total_with_limit(r, k - 1);

    return total_k - total_km1;
}

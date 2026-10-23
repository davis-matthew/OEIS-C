/*
 * A015718  Triangular array T given by rows: T(n,k) = number of partitions of
 *          n into distinct parts, none of which is k (1<=k<=n).
 * https://oeis.org/A015718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, k) where (r, k) are obtained from the n-th element
 * of the row‑major flattening of the triangular array
 *    T(n, k) = number of partitions of n into distinct parts that do NOT
 *              contain the part k (1 ≤ k ≤ n).
 *
 * The algorithm:
 *   1. Convert the linear index n to its corresponding row r and column k:
 *        r is the smallest integer with r(r+1)/2 ≥ n.
 *        k = n – (r-1)r/2.
 *   2. Count the partitions of r into distinct parts that avoid part k.
 *      This is done by a simple 0/1 knapsack DP that skips part k.
 *
 * Complexity:  O(r²) operations, with r ≈ √(2n).  For n ≤ 10 000 this is
 *              well below the 2 s time limit.
 *
 * Notes on types:
 *   - All arithmetic that involves the index n or derived values is cast to
 *     seq_t before multiplication to avoid overflow.
 *   - The DP uses seq_t as the element type, so it works for all supported
 *     integer sizes and for arbitrary‑precision builds.
 */

#include <stdlib.h>
#include <stdint.h>

static inline idx_t find_row(idx_t n)
{
    /* Find the smallest r such that r(r+1)/2 >= n */
    idx_t lo = 0, hi = 1;
    while ((seq_t)hi * ((seq_t)hi + 1) / 2 < n) hi <<= 1;
    idx_t r = hi;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* safety for out of range */
    /* 1. Determine r and k from the linear index n */
    idx_t r = find_row(n);
    seq_t sum_prev = ((seq_t)(r - 1)) * ((seq_t)(r - 1) + 1) / 2;
    idx_t k = (idx_t)(n - sum_prev);   /* 1 ≤ k ≤ r */

    /* 2. DP to count partitions of r into distinct parts that avoid part k */
    seq_t *dp = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failed; return 0 */
    dp[0] = (seq_t)1;

    for (idx_t part = 1; part <= r; ++part) {
        if (part == k) continue;       /* skip the forbidden part */
        for (idx_t sum = r; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[r];
    free(dp);
    return result;
}

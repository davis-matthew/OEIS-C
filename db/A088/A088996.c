/*
 * A088996  Triangle T(n, k) read by rows: T(n, k) = Sum_{j=0..n} binomial(j,
 *          n-k) * |Stirling1(n, n-j)|.
 * https://oeis.org/A088996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 81, u64 n <= 178,
 *            u128 n <= 475, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088996
 *  a(n) = the n-th entry of the triangle T read by rows.
 *  Triangle recurrence:
 *      T(0,0) = 1,
 *      T(n,0) = 0 for n>0,
 *      T(n,k) = n*T(n-1,k-1) + (n-1)*T(n-1,k)   (1 ≤ k ≤ n).
 *
 *  Flattening rule:
 *      let r = ⌊(√(8n+1)−1)/2⌋      // row index
 *      let k = n − r(r+1)/2          // column index
 *  then a(n) = T(r,k).
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square‑root for uint64_t.
 * Returns floor(√x).  Binary search with division to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid) /* mid*mid ≤ x without overflow */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row and column within the triangle */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate two rows of length r+1 */
    size_t len = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;               /* fallback on allocation failure */
    }

    /* Base row T(0,0)=1 */
    prev[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i) prev[i] = (seq_t)0;

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)0;           /* T(i,0)=0 for i>0 */
        for (idx_t j = 1; j <= i; ++j) {
            curr[j] = (seq_t)i * prev[j - 1] + (seq_t)(i - 1) * prev[j];
        }
        /* Copy curr to prev for next iteration */
        for (size_t j = 0; j <= (size_t)i; ++j) {
            prev[j] = curr[j];
        }
    }

    /* Result */
    seq_t result = prev[(size_t)k];

    /* Cleanup */
    free(prev);
    free(curr);
    return result;
}

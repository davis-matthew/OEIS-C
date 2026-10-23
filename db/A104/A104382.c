/*
 * A104382  Triangle, read by rows, where T(n,k) equals number of distinct
 *          partitions of triangular number n*(n+1)/2 into k different
 *          summands for n>=k>=1.
 * https://oeis.org/A104382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of the triangular number r*(r+1)/2
 *            into c distinct summands, where r = ceil((sqrt(8*n+1)-1)/2) (the
 *            row) and c = n - r*(r-1)/2 (the column). The value is returned
 *            as type seq_t
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;          /* 2‑D table for partitions into k parts */
static idx_t dpNmax = 0;          /* current maximum n value stored in dp   */
static idx_t dpKmax = 0;          /* current maximum k value stored in dp   */

static void recompute_dp(idx_t needN, idx_t needK)
{
    if (needN <= dpNmax && needK <= dpKmax) return;   /* nothing to do */

    idx_t newN = needN;
    idx_t newK = needK;
    size_t size = (size_t)(newN + 1) * (newK + 1);

    seq_t *newdp = (seq_t *)realloc(dp, size * sizeof(seq_t));
    if (!newdp) return;   /* allocation failed – give up silently */

    /* zero the new table */
    memset(newdp, 0, size * sizeof(seq_t));

    /* base case: one way to partition 0 into 0 parts */
    newdp[0] = 1;

    /* DP for partitions into exactly k parts (parts may repeat) */
    for (idx_t n = 1; n <= newN; ++n) {
        idx_t kmax = (n < newK) ? n : newK;
        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t val = newdp[(n - 1) * (newK + 1) + (k - 1)];
            if (n >= k) {                       /* use part 'k' once */
                val += newdp[(n - k) * (newK + 1) + k];
            }
            newdp[n * (newK + 1) + k] = val;
        }
    }

    dp = newdp;
    dpNmax = newN;
    dpKmax = newK;
}

/* public API: returns the n-th term of sequence A104382 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used but harmless */

    /* locate the row r and column c of the n-th term */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;            /* guaranteed to finish quickly for n ≤ 10000 */
    idx_t c = n - r * (r - 1) / 2;              /* 1‑based column index   */

    /* triangular number of the row */
    idx_t tri = r * (r + 1) / 2;

    /* value needed from the partition table:
       n' = tri - c*(c-1)/2   (triangular number minus the minimal sum of c distinct parts) */
    idx_t delta = c * (c - 1) / 2;
    idx_t nprime = tri - delta;

    /* ensure the DP table is large enough */
    if (tri > dpNmax || c > dpKmax)
        recompute_dp(tri, c);

    /* fetch the result: partitions of nprime into exactly c parts (repetition allowed) */
    size_t width = dpKmax + 1;
    return dp[nprime * width + c];
}

/*
 * A144886  Lower triangular array called S1hat(4) related to partition number
 *          array A144885.
 * https://oeis.org/A144886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144886
    This function returns the flattened entries of the lower‑triangular
    array S1hat(4) (partitions weighted by (k+2)!/6).  For an index n
    (1‑based) it retrieves the element in row r with r(r‑1)/2 < n ≤ r(r+1)/2,
    where r = ceil( (sqrt(8n+1)-1)/2 ).  The values are computed by a
    straightforward dynamic programme that enumerates all m‑part
    partitions of n and weights each part k by base[k]=(k+2)!/6.
    The computation is memoised: once the array is filled up to row r,
    all future queries with larger indices recompute only up to the
    required row.  For the typical range of indices used in tests (≤10 000)
    the maximum row needed is about 140; the DP therefore runs in
    a fraction of a second.  No external utilities or floating point
    arithmetic are used.
*/

#include <stdint.h>
#include <stddef.h>

/* Build‑time type definitions of seq_t and idx_t are provided by the
   test harness.  They are assumed to support simple arithmetic
   operations, comparison, and assignment. */

#define MAX_ROW 2000        /* sufficient for indices up to 10 000 */

/* DP table: dp[n][m] = weighted sum of m‑part partitions of n.
   base[k] = (k+2)!/6.  All entries are zeroed before recomputation. */
static seq_t dp[MAX_ROW + 1][MAX_ROW + 1] = {0};
static seq_t base[MAX_ROW + 1] = {0};
static idx_t current_row = 0;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    /* Determine row r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* If the DP table does not yet cover this row, recompute it. */
    if (r > current_row) {
        /* Zero all relevant entries.  Using loops guarantees correctness
           even if seq_t is a non‑POD type. */
        for (idx_t i = 0; i <= r; ++i)
            for (idx_t j = 0; j <= i; ++j)
                dp[i][j] = (seq_t)0;

        dp[0][0] = (seq_t)1;      /* empty partition */

        /* Pre‑compute base values up to r */
        for (idx_t k = 1; k <= r; ++k) {
            if (k == 1)
                base[1] = (seq_t)1;                     /* (1+2)!/6 = 1 */
            else
                base[k] = base[k - 1] * ((seq_t)(k) + (seq_t)2);

            /* Update DP using part size k */
            for (idx_t sum = k; sum <= r; ++sum) {
                for (idx_t part = 1; part <= sum; ++part) {
                    seq_t prev = dp[sum - k][part - 1];
                    if (prev != (seq_t)0)
                        dp[sum][part] += prev * base[k];
                }
            }
        }
        current_row = r;
    }

    /* Column index within the row */
    idx_t col = n - (r * (r - 1)) / 2;

    return dp[r][col];
}

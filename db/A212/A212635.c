/*
 * A212635  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with cardinality k of the wheel W_n (n >= 4, 1
 *          <= k <= n).
 * https://oeis.org/A212635
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   binom, prefix_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212635 – irregular triangle read by rows
 *
 * T(n,k): number of dominating subsets of size k in the wheel W_n
 *        (n ≥ 4, 1 ≤ k ≤ n).
 *
 * Recurrence for the triangle coefficients (n ≥ 7, 1 ≤ k ≤ n):
 *
 *   T(n,k) = T(n-1,k-1) + T(n-2,k-1) + T(n-3,k-1) + binom(n-4,k-1)
 *
 * Base rows:
 *
 *   n=4 : 4, 6, 4, 1
 *   n=5 : 1,10,10, 5,1
 *   n=6 : 1,10,20,15, 6,1
 *
 * The OEIS sequence a(n) lists these rows row‑major, with offset 4:
 *
 *   a(4)=4, a(5)=6, a(6)=4, a(7)=1, a(8)=1, a(9)=10, ...
 *
 * The function below implements the recurrence, memoising rows so that
 * each n≥4 can be answered in O(1) time after the first appearance for
 * that n.  The implementation uses the provided binom routine for
 * integer binomial coefficients.
 */

#include <stdlib.h>
#include <string.h>

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/prefix_sum.h"

/* The table of rows.  rows[r] points to an array of length r+1
   (index 0 unused).  rows[0..3] are NULL.                              */
static seq_t **rows = NULL;
static size_t rows_cap = 0;

/* Return the triangle coefficient T(r,k) for r≥4, 1≤k≤r,
   building rows on demand.                                            */
static seq_t get_T(idx_t r, idx_t k)
{
    /* Ensure the table large enough for index r. */
    if (r >= rows_cap) {
        size_t new_cap = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) return 0;          /* allocation failure → zero */
        /* initialise new slots to NULL */
        for (size_t i = rows_cap; i < new_cap; ++i) tmp[i] = NULL;
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Build all missing rows up to r. */
    for (idx_t cur = 4; cur <= r; ++cur) {
        if (rows[(size_t)cur]) continue;   /* already built */

        /* allocate storage for this row */
        rows[(size_t)cur] = (seq_t*)calloc((size_t)cur + 1, sizeof(seq_t));
        if (!rows[(size_t)cur]) return 0;   /* allocation failure → zero */

        if (cur == 4) {
            rows[(size_t)4][1] = (seq_t)4;
            rows[(size_t)4][2] = (seq_t)6;
            rows[(size_t)4][3] = (seq_t)4;
            rows[(size_t)4][4] = (seq_t)1;
        } else if (cur == 5) {
            rows[(size_t)5][1] = (seq_t)1;
            rows[(size_t)5][2] = (seq_t)10;
            rows[(size_t)5][3] = (seq_t)10;
            rows[(size_t)5][4] = (seq_t)5;
            rows[(size_t)5][5] = (seq_t)1;
        } else if (cur == 6) {
            rows[(size_t)6][1] = (seq_t)1;
            rows[(size_t)6][2] = (seq_t)10;
            rows[(size_t)6][3] = (seq_t)20;
            rows[(size_t)6][4] = (seq_t)15;
            rows[(size_t)6][5] = (seq_t)6;
            rows[(size_t)6][6] = (seq_t)1;
        } else { /* recurrence for n ≥ 7 */
            for (idx_t j = 1; j <= cur; ++j) {
                seq_t val = 0;
                if (j-1 <= cur-1) val += rows[(size_t)(cur-1)][j-1];
                if (j-1 <= cur-2) val += rows[(size_t)(cur-2)][j-1];
                if (j-1 <= cur-3) val += rows[(size_t)(cur-3)][j-1];
                if (j-1 <= cur-4) val += binom(cur-4, j-1);
                rows[(size_t)cur][j] = val;
            }
        }
    }
    return rows[(size_t)r][k];
}

seq_t a(idx_t n)
{
    /* a(n) defined for n ≥ 4.  For n < 4 we return 0. */
    if (n < 4) return 0;

    /* Convert flat index n (offset 4) to row/column pair. */
    idx_t pos = n - 3;           /* position among all entries, 1‑based */

    /* Find smallest r such that prefix_sum(r) ≥ pos */
    idx_t lo = 4, hi = 4;
    while (prefix_sum(hi) < pos) hi <<= 1;          /* exponential bound */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prefix_sum(mid) >= pos) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;

    idx_t prev_total = prefix_sum(r - 1);
    idx_t k = pos - prev_total;   /* 1‑based column index */

    return get_T(r, k);
}

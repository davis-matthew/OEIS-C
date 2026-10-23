/*
 * A013988  Triangle read by rows, the inverse Bell transform of
 *          n!*binomial(5,n) (without column 0).
 * https://oeis.org/A013988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th entry of triangle T with T(1,1)=1 and
 *            recurrence T(n+1,m) = (6*n - m)*T(n,m) + T(n,m-1) (n≥m≥1) a(n)
 *            is read row‑by‑row, offset 1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 91, u128 n <= 276,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* maximum row we support comfortably (≈ sqrt(2⋅max_n) ). */
#define MAX_R 200

seq_t a(idx_t n)
{
    /* Handle the undefined region below the offset. */
    if (n == 0) return 0;

    /* Find the row r such that r(r+1)/2 ≥ n.  */
    idx_t r = 1;
    idx_t cum = 1;               /* 1 = sum_{k=1}^1 k */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Static storage for the two most recent rows.
       The size is large enough for all rows up to MAX_R. */
    static seq_t prev[MAX_R + 1];
    static seq_t cur [MAX_R + 1];

    /* Initialize the first row (n=1).  */
    prev[0] = 0;
    prev[1] = 1;                 /* T(1,1) = 1 */

    /* If we are looking at the first row, simply return its entry. */
    if (r == 1)
        return prev[1];

    /* Compute rows 2 … r using the recurrence.  */
    for (idx_t curr = 2; curr <= r; ++curr) {
        cur[0] = 0;              /* columns start at 1 */
        cur[curr] = 1;           /* T(curr,curr) = 1 */

        for (idx_t m = 1; m < curr; ++m) {
            seq_t factor = 6 * (seq_t)(curr - 1) - (seq_t)m;
            cur[m] = factor * prev[m] + prev[m - 1];
        }

        /* Prepare prev for the next iteration: copy cur into prev. */
        for (idx_t i = 0; i <= curr; ++i)
            prev[i] = cur[i];
    }

    /* At this point cur holds the values for row r. */
    /* Determine column index within this row. */
    idx_t first = (r - 1) * r / 2 + 1;   /* first index of row r */
    idx_t j = n - first + 1;             /* 1‑based column number */

    return cur[j];
}

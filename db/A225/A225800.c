/*
 * A225800  Triangle of rising diagonals of A011973 (with rows displayed as
 *          centered text).
 * https://oeis.org/A225800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the flattened triangle of rising
 *            diagonals of A011973. Triangle entry: a(row,k) = binomial( 2*row
 *            - k - 2 - 3*floor(k/2), floor(k/2) ), where row ≥ 1 and k = 0 …
 *            floor((2*row-1)/3). The linear index n is located by cumulating
 *            row lengths: len(row) = floor((2*row-1)/3) + 1
 * Range:     u8 n <= 71, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1; return 0 for safety */

    /* static cache of cumulative element counts per row */
    static idx_t *cum        = NULL;    /* cum[row] = total elements up to this row */
    static size_t cap        = 0;       /* current capacity of cum array      */
    static idx_t last_row    = 0;       /* last row that has been computed   */
    static idx_t cum_last    = 0;       /* cumulative total up to last_row   */

    /* initialize the cache on first call */
    if (cum == NULL) {
        cap = 64;
        cum = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cum) return 0;                    /* allocation failed */
        cum[0] = 0;                            /* zero rows -> zero elements      */
    }

    /* extend precomputation until we have at least n elements */
    while (cum_last < n) {
        idx_t row = last_row + 1;
        idx_t len = ((2 * row - 1) / 3) + 1;    /* number of elements in this row */

        /* enlarge capacity if needed */
        if (row >= cap) {
            size_t new_cap = cap * 2;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) return 0;                    /* allocation failed */
            cum = tmp;
            cap = new_cap;
        }

        cum_last += len;
        cum[row] = cum_last;      /* store cumulative total for this row */
        last_row = row;
    }

    /* binary search to find the row containing n */
    idx_t low  = 1;
    idx_t high = last_row;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;                       /* row number (1‑based) */

    /* index of first element in this row */
    idx_t start_of_row = cum[row - 1] + 1;
    idx_t k = n - start_of_row;            /* 0‑based column index */

    /* compute the triangle entry using the OEIS formula */
    idx_t m = k / 2;                       /* floor(k/2) */
    idx_t numerator = 2 * row - k - 2 - 3 * m;
    return binomial(numerator, m);
}

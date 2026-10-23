/*
 * A177815  Triangle read by rows: binomial(n, m^3).
 * https://oeis.org/A177815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r, j^3) where r is the row index and j is the
 *            offset within that row (0‑based). Each row r contains
 *            binomial(r, m^3) for m = 0 .. floor(r^(1/3))
 * Range:     u8 n <= 25, u16 n <= 40, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"
#include "../utils/binomial.h"

/* Cumulative number of terms up to and including each row.
   cumulative[i] = sum_{k=0}^{i} (floor(k^(1/3)) + 1) */
static idx_t *cumulative = NULL;
static size_t cum_size = 0;

static void ensure_cumulative(idx_t n)
{
    if (cum_size == 0) {
        cumulative = (idx_t*)malloc(sizeof(idx_t));
        if (!cumulative) return;
        cumulative[0] = 1;          /* row 0 has length 1 */
        cum_size = 1;
    }

    /* Extend cumulative array until cum[cum_size-1] > n */
    while (cumulative[cum_size - 1] <= n) {
        idx_t row = cum_size;                   /* next row to compute */
        idx_t m   = int_cube_root(row);
        idx_t len = m + 1;                      /* number of entries in this row */
        idx_t new_sum = cumulative[cum_size - 1] + len;
        idx_t *tmp = (idx_t*)realloc(cumulative, sizeof(idx_t) * (cum_size + 1));
        if (!tmp) break;
        cumulative = tmp;
        cumulative[cum_size] = new_sum;
        ++cum_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cumulative(n);

    /* Binary search to find the row that contains position n */
    idx_t lo = 0;
    idx_t hi = (idx_t)(cum_size - 1);
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cumulative[mid] > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;
    idx_t prev_sum = (row == 0) ? 0 : cumulative[row - 1];
    idx_t offset = n - prev_sum;              /* offset within the row */

    /* Compute binomial(row, j^3) where j = offset */
    idx_t j3 = offset * offset * offset;
    return binomial(row, j3);
}

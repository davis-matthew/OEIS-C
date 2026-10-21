/*
 * A212633  Triangle read by rows: T(n,k) is the number of dominating subsets
 *          with cardinality k of the path tree P_n (n>=1, 1<=k<=n).
 * https://oeis.org/A212633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * T(r,k) is the coefficient of x^k in the
 *            domination polynomial of * the path P_r (the r‑th row of
 *            triangle A212633), * * row r starts with k=1 and ends with k=r,
 *            * * T(r,k) satisfies * T(r,k) = T(r‑1,k‑1) + T(r‑2,k‑1) +
 *            T(r‑3,k‑1) for r≥4 and all k, * * initial rows are * T(1...
 * Range:     u8 n <= 72, u16 n <= 222, u32 n <= 803, u64 n <= 2971,
 *            u128 n <= 8787, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the smallest r such that r*(r+1)/2 ≥ idx.  idx is 1‑based. */
static inline idx_t find_row(idx_t idx)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until it encloses idx */
    while ((unsigned __int128)high * (high + 1) / 2 < idx) {
        if (high > (idx_t)(-1) / 2) {   /* avoid overflow */
            high = (idx_t)(-1);
            break;
        }
        high <<= 1;
    }
    /* Binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Cache of computed rows: rows[1] .. rows[max_row] are valid */
static seq_t **rows = NULL;
static idx_t   max_row = 0;
static size_t  rows_cap = 0;

/* Ensure rows up to target_row are computed and cached. */
static void ensure_rows(idx_t target_row)
{
    if (target_row <= max_row)
        return;

    /* Grow rows array if needed. */
    if (rows_cap < (size_t)target_row + 1) {
        size_t new_cap = rows_cap ? rows_cap * 2 : target_row + 1;
        if (new_cap < (size_t)target_row + 1)
            new_cap = target_row + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        rows = tmp;
        rows_cap = new_cap;
        for (size_t i = max_row + 1; i <= target_row; ++i)
            rows[i] = NULL;
    }

    /* Compute rows from (max_row+1) up through target_row. */
    for (idx_t r = max_row + 1; r <= target_row; ++r) {
        /* row r has indices 0..r ; we ignore 0 for simplicity */
        rows[r] = (seq_t*)calloc((size_t)r + 1, sizeof(seq_t));
        if (!rows[r]) abort();

        if (r == 1) {
            rows[1][1] = 1;
        } else if (r == 2) {
            rows[2][1] = 2;
            rows[2][2] = 1;
        } else if (r == 3) {
            rows[3][1] = 1;
            rows[3][2] = 3;
            rows[3][3] = 1;
        } else {
            /* Recurrence T(r,k) = T(r-1,k-1)+T(r-2,k-1)+T(r-3,k-1) */
            for (idx_t k = 1; k <= r; ++k) {
                seq_t sum = 0;
                /* row r-1 always has k-1 ≤ r-1 */
                sum += rows[r-1][k-1];
                /* Add from row r-2 if it exists for this k */
                if (k-1 <= r-2)
                    sum += rows[r-2][k-1];
                /* Add from row r-3 if it exists for this k */
                if (k-1 <= r-3)
                    sum += rows[r-3][k-1];
                rows[r][k] = sum;
            }
        }
    }
    max_row = target_row;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used, offset is 1 */

    idx_t r = find_row(n);
    idx_t prev_cumulative = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - prev_cumulative;     /* 1‑based column index */

    ensure_rows(r);

    return rows[r][k];
}

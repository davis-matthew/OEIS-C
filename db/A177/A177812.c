/*
 * A177812  Triangular array read by rows: binomial(n,k^2),
 *          k=0..floor(sqrt(n)).
 * https://oeis.org/A177812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r, k²), where r is the row containing n * and k
 *            is the position within that row (0 ≤ k ≤ ⌊√r⌋)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

static idx_t *cum      = NULL;      /* cumulative start indices of rows   */
static size_t cum_len  = 0;          /* number of entries in cum (≥ 2)    */
static size_t cum_cap  = 0;          /* current capacity of cum           */

/* Integer square‑root (floor) for idx_t, suitable for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = x;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Ensure that cum has an entry for every row up to row n (i.e.,
 * cum[i] ≤ n < cum[i+1] holds for i from 0 to cum_len-2). */
static void ensure_cum(idx_t n)
{
    if (cum == NULL) {            /* first time: initialise */
        cum_cap = 16;
        cum     = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum) abort();
        cum[0] = 0;               /* start index of row 0 */
        cum_len = 1;
    }

    /* Extend cum until the start index of the next row exceeds n */
    while (1) {
        idx_t last_index = cum_len - 1;          /* row number of last row */
        idx_t new_start = cum[last_index] + isqrt_u64(last_index) + 1;

        if (cum_len == cum_cap) {                /* grow array */
            cum_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(cum, cum_cap * sizeof(idx_t));
            if (!tmp) abort();
            cum = tmp;
        }
        cum[cum_len] = new_start;                /* store start of next row */
        cum_len++;

        if (new_start > n) break;
    }
}

/* Binary search to find the row r such that cum[r] ≤ n < cum[r+1] */
static idx_t row_for_idx(idx_t n)
{
    /* Assumes cum_len ≥ 2 and cum[cum_len-1] > n */
    idx_t low  = 0;
    idx_t high = cum_len - 2;      /* last valid row index */

    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (cum[mid] <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    ensure_cum(n);
    idx_t r   = row_for_idx(n);
    idx_t pos = n - cum[r];         /* position within the row: k */
    idx_t k   = pos;
    idx_t kk  = k * k;               /* integer square of k */
    return binomial(r, kk);
}

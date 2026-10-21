/*
 * A096646  Triangle (read by rows) where the number of row entries increases
 *          by steps of 2 and the entries are stacked in a rectangular
 *          fashion. The end entries = 1. Rest of entries in the n-th row are
 *          the sum of the entries directly above and to the left and right in
 *          all previous rows (total of 3*(n-1) ent...
 * https://oeis.org/A096646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096646 – Triangle read by rows, end entries = 1, the remaining
   entries are the sum of the three entries directly above and to the
   left/right in all previous rows (total of 3·(n‑1) contributions).  */

/* Includes present in all builds */
#include <stdlib.h>
#include <stdint.h>

/* Utility: floor(sqrt(n)) for unsigned 64‑bit inputs.
   Returns the largest r with r*r <= n. */
static idx_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);          /* > sqrt(U64) */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Utility: ceil(sqrt(n)) for unsigned 64‑bit inputs.
   Returns the smallest r with r*r >= n. */
static idx_t ceil_sqrt_u64(uint64_t n)
{
    idx_t r = floor_sqrt_u64(n);
    if ((unsigned __int128)r * r < n) ++r;
    return r;
}

/* Storage for the triangle.  triangle[r] points to an array of length
   2*r‑1 (0‑based indices).  The array is 1‑based in the sense that
   row 1 is stored at triangle[1] (triangle[0] is unused). */
static seq_t **triangle = NULL;
static idx_t cache_cap = 0;   /* capacity of the triangle pointer array     */
static idx_t cache_rows = 0;  /* number of rows actually stored (0 if none) */

/* Build rows up to and including row_needed (inclusive). */
static void build_rows(idx_t row_needed)
{
    if (row_needed <= cache_rows) return;

    /* Grow the pointer array if necessary. */
    if (cache_cap < row_needed + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < row_needed + 1) new_cap = row_needed + 1;
        triangle = (seq_t **)realloc(triangle, sizeof(seq_t *) * new_cap);
        cache_cap = new_cap;
    }

    /* Build missing rows. */
    for (idx_t r = cache_rows + 1; r <= row_needed; ++r) {
        idx_t len = 2 * r - 1;
        seq_t *row = (seq_t *)malloc((size_t)len * sizeof(seq_t));
        if (!row) return;      /* allocation failure – abort quietly */

        /* First and last entries are 1. */
        row[0] = (seq_t)1;
        row[len - 1] = (seq_t)1;

        /* Compute interior entries (if any). */
        for (idx_t j = 1; j < len - 1; ++j) {          /* 0‑based j */
            seq_t sum = 0;
            for (idx_t k = 1; k < r; ++k) {             /* previous row k */
                idx_t d = r - k;                       /* distance to row k */
                idx_t prev_len = 2 * k - 1;
                /* indices in row k (0‑based) that arise from the
                   current position j in row r.  Use signed arithmetic
                   to detect negative values. */
                int64_t i1 = (int64_t)j - (int64_t)d - 1;
                int64_t i2 = (int64_t)j - (int64_t)d;
                int64_t i3 = (int64_t)j - (int64_t)d + 1;
                if (i1 >= 0 && (idx_t)i1 < prev_len)
                    sum += triangle[k][(idx_t)i1];
                if (i2 >= 0 && (idx_t)i2 < prev_len)
                    sum += triangle[k][(idx_t)i2];
                if (i3 >= 0 && (idx_t)i3 < prev_len)
                    sum += triangle[k][(idx_t)i3];
            }
            row[j] = sum;
        }

        triangle[r] = row;
        cache_rows = r;
    }
}

/* Main function requested by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    idx_t row = ceil_sqrt_u64((uint64_t)n);
    build_rows(row);               /* ensure the row is present */

    /* Position within the row: rows before this one contain (row‑1)² terms. */
    idx_t pos = n - (row - 1) * (row - 1);
    return triangle[row][pos - 1];
}

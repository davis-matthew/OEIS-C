/*
 * A084600  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1+x+2x^2)^n for n >= 0.
 * https://oeis.org/A084600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^{n - floor(sqrt(n))^2} in (1 + x +
 *            2x^2)^{floor(sqrt(n))}. The sequence is the triangle read by
 *            rows of the coefficients of (1 + x + 2x^2)^n
 * Range:     u8 n <= 40, u16 n <= 109, u32 n <= 342, u64 n <= 1195,
 *            u128 n <= 4561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---- static storage for computed rows ---- */
static seq_t **row_ptrs = NULL;          /* pointers to coefficient arrays */
static size_t rows_alloc = 0;            /* allocated size of row_ptrs[] */
static size_t rows_computed = 0;         /* highest row index computed (inclusive) */

/* ---- integer square root for uint64_t (return floor(sqrt(n))) ---- */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 0, high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* use 128-bit to avoid overflow */
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* ---- ensure that row r is computed ---- */
static void ensure_row(idx_t r)
{
    if (r < rows_computed) return;                /* already present */

    /* grow row_ptrs array if necessary */
    if ((size_t)r + 1 > rows_alloc) {
        size_t new_alloc = (rows_alloc == 0) ? (r + 1) : rows_alloc * 2;
        if (new_alloc < (size_t)r + 1) new_alloc = r + 1;
        row_ptrs = (seq_t **)realloc(row_ptrs, new_alloc * sizeof(seq_t *));
        rows_alloc = new_alloc;
    }

    /* compute rows up to r */
    for (idx_t i = rows_computed; i <= r; ++i) {
        size_t len = (size_t)(2 * i + 1);
        row_ptrs[i] = (seq_t *)malloc(len * sizeof(seq_t));
        if (i == 0) {
            row_ptrs[0][0] = (seq_t)1;
            continue;
        }
        /* recurrence: coeff[i][p] = coeff[i-1][p] + coeff[i-1][p-1] + 2*coeff[i-1][p-2] */
        size_t prev_len = (size_t)(2 * (i - 1) + 1);
        for (size_t p = 0; p < len; ++p) {
            seq_t a = 0, b = 0, c = 0;
            if (p < prev_len) a = row_ptrs[i - 1][p];
            if (p > 0 && p - 1 < prev_len) b = row_ptrs[i - 1][p - 1];
            if (p > 1 && p - 2 < prev_len) c = row_ptrs[i - 1][p - 2];
            row_ptrs[i][p] = a + b + ((seq_t)2 * c);
        }
    }
    rows_computed = r + 1;
}

/* ---- main function returning the nth term of the flattened triangle ---- */
seq_t a(idx_t n)
{
    /* determine which row contains index n */
    idx_t r = isqrt_u64(n);          /* floor(sqrt(n)) */
    idx_t pos = n - r * r;           /* position within that row */

    ensure_row(r);                   /* make sure row r is ready */

    return row_ptrs[r][pos];         /* coefficient at required position */
}

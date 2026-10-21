/*
 * A225473  Triangle read by rows, k!*S_4(n, k) where S_m(n, k) are the
 *          Stirling-Frobenius subset numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T where * T(0,0)=1,
 *            T(n,0)=3*T(n-1,0), and for k≥1 *
 *            T(n,k)=4*k*T(n-1,k-1)+(3+4*k)*T(n-1,k). This triangle is read *
 *            row‑by‑row (row 0, then row 1, etc.). The index n runs over the
 *            * flattened list of triangle entries, starting with a(0)=1
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 110, u128 n <= 288,
 *            big any n
 * Helpers:   ensure_row_storage
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_row_storage.h"

/*--------------------  Helper for row storage  --------------------*/

/*--------------------  Global cache  --------------------*/

static seq_t **_tri_rows = NULL;
static size_t   _rows_allocated = 0;
static size_t   _rows_computed = 0;

/* Allocate a new row of length len */
static seq_t *allocate_row(size_t len)
{
    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    if (!row) abort();             /* fatal on allocation failure */
    return row;
}

/* Ensure all rows up to index r (inclusive) are computed and cached */
static void _ensure_rows_up_to(size_t r)
{
    if (_rows_computed > r) return;

    /* grow the cache of row pointers if necessary */
    if (_rows_allocated <= r) {
        size_t new_size = (r + 1) * 2;
        seq_t **tmp = (seq_t **)realloc(_tri_rows, new_size * sizeof(seq_t *));
        if (!tmp) abort();          /* allocation failure */
        _tri_rows = tmp;
        _rows_allocated = new_size;
    }

    /* compute missing rows */
    for (size_t i = _rows_computed; i <= r; ++i) {
        size_t len = i + 1;
        seq_t *row = allocate_row(len);

        if (i == 0) {
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = _tri_rows[i - 1];

            /* k = 0 */
            row[0] = (seq_t)3 * prev[0];

            /* k ≥ 1 */
            for (size_t k = 1; k <= i; ++k) {
                seq_t term1 = (seq_t)4 * (seq_t)k * prev[k - 1];
                seq_t term2 = 0;
                if (k <= i - 1) {
                    term2 = ((seq_t)3 + (seq_t)4 * (seq_t)k) * prev[k];
                }
                row[k] = term1 + term2;
            }
        }

        _tri_rows[i] = row;
    }

    _rows_computed = r + 1;
}

/* Find row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static size_t _row_of_index(idx_t n)
{
    size_t lo = 0, hi = (size_t)n;  /* n fits into size_t for our n ≤ 10⁴ */
    while (lo < hi) {
        size_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t cnt = (__uint128_t)mid * (mid + 1) / 2;
        if (cnt <= (unsigned long long)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    size_t r = _row_of_index(n);
    /* offset of the first element in row r */
    idx_t offset = (idx_t)((__uint128_t)r * (r + 1) / 2);
    size_t k = (size_t)(n - offset);

    _ensure_rows_up_to(r);
    return _tri_rows[r][k];
}
